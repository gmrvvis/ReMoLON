#include "ReMoLON_Util/SecureServer.h"

#include <Poco/Net/Context.h>
#include <Poco/Net/SecureServerSocket.h>

#include <iostream>

namespace remolonUtil
{
  SecureServer::SecureServer ( const TServerConfig & serverConfig_ )
  {
    _bindIPAddress = serverConfig_._bindToAddress;
    _bindPort = serverConfig_._bindToPort;
    _keyFilePath = serverConfig_._keyFilePath;
    _certFilePath = serverConfig_._certFilePath;
    _caFilePath = serverConfig_._caFilePath;
  }

  SecureServer::~SecureServer ( )
  {
    shutDown ( );
  }

  void SecureServer::start ( )
  {
    _active = true;

    initializeSocket ( );

    _acceptorThread = std::thread( &SecureServer::acceptLoop, this );
    _selectorWorkers.emplace_back ( std::make_unique< remolonUtil::SelectorThread > ( _packetThreadPool, 
                                                                                      _packetHandler ) );
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
      Poco::Net::Context::VERIFY_NONE, 
      9, 
      false, 
      "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
    );

    context->enableSessionCache ( true );

    Poco::Net::SocketAddress sa ( _bindIPAddress, _bindPort );

    _serverSocket = std::make_unique < Poco::Net::SecureServerSocket >( sa, 64, context );
  }

  void SecureServer::acceptLoop ( )
  {
    Poco::Net::ServerSocket * serverSock = _serverSocket.get ( );
    try
    {
      while ( _active )
      {
        try
        {
          Connection con ( serverSock->acceptConnection ( ) );
          con.completeHandshake ( );
          //std::cout << "Connection accepted from " << ss.address ( ).toString( ) << std::endl;
          assignConnectionToWorker ( con );
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

  void SecureServer::assignConnectionToWorker ( Connection & con_ )
  {
    SelectorThread * lessLoaded;
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

    lessLoaded->addConnection ( &con_ );
  }

  void SecureServer::shutDown ( )
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
        if ( _acceptorThread.joinable ( ) )
        {
          _acceptorThread.join ( );
        }
      }
      catch( std::exception & e )
      {
        // ignore
      }
    }
  }
}