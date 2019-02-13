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
 
#include "ReMoLON_Util/Connection.h"

#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/Net/KeyFileHandler.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/ConsoleCertificateHandler.h>
#include <Poco/Net/SSLManager.h>

namespace remolonUtil
{
  Connection::Connection ( )
   : _owningPollset ( nullptr )
  {
  }

  int Connection::getSockFD ( )
  {
    if ( _socket.get ( ) )
    {
      return _socket.get ( )->impl ( )->sockfd ( );
    }

    return -1;
  }

  Poco::Net::StreamSocket * Connection::getSocket ( )
  {
    return _socket.get ( );
  }

  void Connection::setPollSetSource ( Poco::Net::PollSet * pollSetSource_ )
  {
    _owningPollset = pollSetSource_;
  }

  void Connection::sendPacket ( SendablePacketPtr & sendable_ )
  {
    std::unique_lock < std::mutex > lock ( _sendMtx );

    _sendPackets.push ( std::move ( sendable_ ) );
    
    if ( _owningPollset )
    {
      _owningPollset->update ( *(_socket.get ( )), Poco::Net::PollSet::POLL_WRITE | Poco::Net::PollSet::POLL_READ );
    }
  }

  SendablePacketPtr Connection::pollSendablePacket ( )
  {
    std::unique_lock < std::mutex > lock ( _sendMtx );

    if ( _sendPackets.empty ( ) )
    {
      // Remove write flag from select polling

      if ( _owningPollset )
      {
        _owningPollset->update ( *(_socket.get ( )), Poco::Net::PollSet::POLL_READ );
      }
      return nullptr;
    }

    SendablePacketPtr nextPacket = std::move ( _sendPackets.front ( ) );
    _sendPackets.pop ( );
    
    return nextPacket;
  }

  void Connection::endConnection ( )
  {
    std::unique_lock < std::mutex > lock ( _sendMtx );
    while ( !_sendPackets.empty ( ) )
    {
      _sendPackets.pop ( );
    }

    if ( _socket.get ( ) )
    {
      _socket.get ( )->close ( );
    }
  }

  std::string Connection::localAddress ( )
  {
    Poco::Net::StreamSocket * socket = _socket.get ( );
    if ( socket )
    {
      return socket->address ( ).host ( ).toString ( );
    }

    return "";
  }

  std::string Connection::clientAddress ( )
  {
    Poco::Net::StreamSocket * socket = _socket.get ( );
    if ( socket )
    {
      return socket->peerAddress ( ).host ( ).toString ( );
    }

    return "";
  }

  RawConnection::RawConnection ( const std::string & address_, uint16_t port_ )
   : Connection ( )
  {
    Poco::Net::SocketAddress sa ( address_, port_ );
    _socket = std::make_unique < Poco::Net::StreamSocket > ( sa );
  }

  RawConnection::RawConnection ( const Poco::Net::Socket & socket_ )
   : Connection ( )
  {
    _socket = std::make_unique < Poco::Net::StreamSocket > ( socket_ );
    _socket.get ( )->setReceiveTimeout ( Poco::Timespan ( 0, 10000 ));
  }

  SecureConnection::SecureConnection ( const std::string & address_, uint16_t port_,
                                       const std::string & keyFile_,
                                       const std::string & certFile_,
                                       const std::string & caFile_ )
   : Connection ( )
  {
    Poco::SharedPtr<Poco::Net::PrivateKeyPassphraseHandler> keyHandler = new Poco::Net::KeyFileHandler(false);
    Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> invalidCertHandler = new Poco::Net::ConsoleCertificateHandler(true);

    Poco::Net::Context::Ptr context = new Poco::Net::Context
    (
      Poco::Net::Context::CLIENT_USE, 
      keyFile_,
      certFile_,
      caFile_, 
      Poco::Net::Context::VERIFY_NONE, 
      9, 
      false, 
      "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
    );

    context->enableSessionCache(true);

    Poco::Net::SSLManager::instance().initializeClient(keyHandler, invalidCertHandler, context);

    Poco::Net::SocketAddress sa ( address_, port_ );

    _socket = std::make_unique < Poco::Net::SecureStreamSocket > ( sa, context );
    Poco::Net::SecureStreamSocket * ssock = static_cast< Poco::Net::SecureStreamSocket * > ( _socket.get ( ) );
    ssock->setPeerHostName ( address_ );
    ssock->completeHandshake ( );
  }

  SecureConnection::SecureConnection ( const Poco::Net::Socket & socket_ )
   : Connection ( )
  {
    _socket = std::make_unique < Poco::Net::SecureStreamSocket > ( socket_ );
    _socket.get ( )->setReceiveTimeout ( Poco::Timespan ( 0, 10000 ));
    Poco::Net::SecureStreamSocket * ssock = static_cast< Poco::Net::SecureStreamSocket * > ( _socket.get ( ) );
    ssock->completeHandshake ( );
  }
}