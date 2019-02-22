/*
 * Copyright (c) 2019 CCS/UPM - GMRV/URJC.
 *
 * Authors: Nadir Román Guerrero <nadir.roman@urjc.es>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "ReMoLON_Util/Server.h"

#include <Poco/Net/Context.h>
#include <Poco/Net/SecureServerSocket.h>

#include <iostream>

namespace remolonUtil
{
  AbstractServer::AbstractServer ( const std::string & address_, std::uint16_t port_ )
   : _bindIPAddress ( address_ )
   , _bindPort ( port_ )
  {
  }

  AbstractServer::~AbstractServer ( )
  {
    shutDown ( );
  }

  void AbstractServer::start ( bool blocking_ )
  {
    _active = true;
    _blocking = blocking_;

    initializeSocket ( );

    _selectorWorkers.emplace_back ( std::unique_ptr < SelectorThread > ( new SelectorThread ( _packetThreadPool, 
                                                                                              _packetHandler ) ) );                                                                                  

    if ( blocking_ )
    {
      acceptLoop ( );
    }
    else
    {
      _acceptorThread = std::thread( &SecureServer::acceptLoop, this );
      _acceptorThread.detach ( );
    }
  }

  void AbstractServer::acceptLoop ( )
  {
    Poco::Net::ServerSocket * serverSock = _serverSocket.get ( );
    try
    {
      while ( _active )
      {
        try
        {
          std::unique_ptr < Connection > newCon = createNewConnection ( serverSock->acceptConnection ( ) );
          assignConnectionToWorker ( newCon );
        }
        catch ( std::exception & ie )
        {
          if ( _active )
          {
            std::cerr << "Error InstanceServer::acceptLoop() while accepting connection: " << ie.what() << std::endl;
          }
        }
      }
    }
    catch ( Poco::Exception & e )
    {
      if ( _active )
      {
        std::cerr << "ERROR InstanceServer::acceptLoop() during loop: " << e.displayText() << std::endl;
      }
    }
  }

  void AbstractServer::assignConnectionToWorker ( ConnectionPtr & con_ )
  {
    //###SelectorThread * lessLoaded;
    SelectorThread * lessLoaded = nullptr;
    
    std::size_t lessLoad = 999999;
    for ( auto & worker : _selectorWorkers )
    {
      size_t workerLoad = worker.get ( )->getNumConnections ( );
      if( workerLoad < lessLoad)
      {
        lessLoad = workerLoad;
        lessLoaded = worker.get ( );
      }
    }

    if ( lessLoaded != nullptr ) lessLoaded->addConnection ( con_ );
    else std::cerr << "ERROR InstanceServer::assignConnectionToWorker() no lessWorkLoad found" << std::endl;
  }

  void AbstractServer::shutDown ( )
  {
    if ( _active )
    {
      try
      {
        // Shutdown connection acceptor worker
        _active = false;

        // Shutdown workers
        for ( auto & worker: _selectorWorkers )
        {
          worker.get ( )->shutDown ( );
        }

        _packetThreadPool.shutDown ( );

        // Unload the ssl context
        Poco::Net::uninitializeSSL ( );

      
        // Close the server socket
        //_serverSocket.get ( )->close ( );
        // SSL connections not finishing using standard close ( ) method
        _serverSocket.get ( )->impl ( )->shutdown ( );
      }
      catch( std::exception & e )
      {
        // ignore
      }
    }
  }

  // =============================================================================================

  RawServer::RawServer ( const std::string & address_, std::uint16_t port_ )
   : AbstractServer ( address_, port_ )
  {
  }

  std::unique_ptr < Connection > RawServer::createNewConnection ( const Poco::Net::Socket & socket_ )
  {
    std::unique_ptr < Connection > 
    newConnection = std::unique_ptr < RawConnection > ( new RawConnection  ( socket_ ) );

    return ( newConnection );
  }

  void RawServer::initializeSocket ( )
  {
    Poco::Net::SocketAddress sa ( _bindIPAddress, _bindPort );
    _serverSocket = std::unique_ptr < Poco::Net::ServerSocket > ( new Poco::Net::ServerSocket ( sa ) );
  }

  // =============================================================================================

  SecureServer::SecureServer ( const std::string & address_, std::uint16_t port_,
                               const std::string & keyFile_,
                               const std::string & certFile_,
                               const std::string & caFile_ )
   : AbstractServer ( address_, port_ )
   , _keyFilePath ( keyFile_ )
   , _certFilePath ( certFile_ )
   , _caFilePath ( caFile_ )
  {
  }

  std::unique_ptr < Connection > SecureServer::createNewConnection ( const Poco::Net::Socket & socket_ )
  {
    std::unique_ptr < Connection > 
    newConnection = std::unique_ptr < SecureConnection > ( new SecureConnection ( socket_ ) );

    return ( newConnection );
  }

  void SecureServer::initializeSocket ( )
  {
    Poco::Net::initializeSSL ( );

    Poco::Net::Context::Ptr context = new Poco::Net::Context
    (
      Poco::Net::Context::SERVER_USE, 
      _keyFilePath,
      _certFilePath,
      _caFilePath, 
      Poco::Net::Context::VERIFY_STRICT, 
      9, 
      false, 
      "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
    );

    context->enableSessionCache ( true );

    Poco::Net::SocketAddress sa ( _bindIPAddress, _bindPort );

    _serverSocket = std::unique_ptr < Poco::Net::SecureServerSocket > ( new Poco::Net::SecureServerSocket ( sa, 64, context ) );
  }
}
