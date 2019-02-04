#ifndef __REMOLON_UTIL_CONNECTION_H__
#define __REMOLON_UTIL_CONNECTION_H__

#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/PollSet.h>

#include <queue>
#include <memory>
#include <mutex>
#include <string>

#include "ReMoLON_Util/Packet.h"

namespace remolonUtil
{
  class Connection : public Poco::Net::SecureStreamSocket
  {
    public:
      Connection ( const std::string & ip_,
                   const unsigned int & port_,
                   Poco::Net::Context::Ptr & sslContext );
      // Server side Connection
      Connection ( const Poco::Net::Socket & socket_ );

      int getSockFD ( void );

      void setPollSetSource ( Poco::Net::PollSet * pollSetSource_ );

      void sendPacket ( SendablePacketPtr & sendable_ );

      SendablePacketPtr pollSendablePacket ( );

      void endConnection ( );
    private:
      std::queue < SendablePacketPtr > _sendPackets;

      std::mutex _sendMtx;

      Poco::Net::PollSet * _owningPollset;
  };

  typedef std::unique_ptr < Connection > ConnectionPtr;
}

#endif