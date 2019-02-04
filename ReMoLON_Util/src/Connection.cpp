#include "ReMoLON_Util/Connection.h"

#include <Poco/Net/SocketAddress.h>


namespace remolonUtil
{
  Connection::Connection ( const std::string & ip_,
                           const unsigned int & port_,
                           Poco::Net::Context::Ptr & sslContext )
   : Poco::Net::SecureStreamSocket ( Poco::Net::SocketAddress ( ip_, port_ ), sslContext )
   , _owningPollset ( nullptr )
  {
    setPeerHostName ( ip_ );
    completeHandshake ( );
  }                  

  Connection::Connection ( const Poco::Net::Socket & socket_ )
   : Poco::Net::SecureStreamSocket ( socket_ )
   , _owningPollset ( nullptr )
  {
    setReceiveTimeout ( Poco::Timespan ( 0, 50000 ));
  }

  int Connection::getSockFD ( )
  {
    return sockfd ( );
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
      _owningPollset->update ( *this, Poco::Net::PollSet::POLL_WRITE | Poco::Net::PollSet::POLL_READ );
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
        _owningPollset->update ( *this, Poco::Net::PollSet::POLL_READ );
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

    close ( );
  }
}