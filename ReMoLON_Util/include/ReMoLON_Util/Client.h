#ifndef __REMOLON_UTIL_SECURECLIENT__
#define __REMOLON_UTIL_SECURECLIENT__

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

#include "ReMoLON_Util/Packet.h"
#include "ReMoLON_Util/Connection.h"
#include "ReMoLON_Util/Config.h"
#include "ReMoLON_Util/ByteBuffer.h"
#include "ReMoLON_Util/PacketHandler.h"

namespace remolonUtil
{
  class Client
  {
    public:
      Client ( const std::string & address_, std::uint16_t port_ );
      ~Client ( );
      
      void connect ( );
      void sendPacket ( SendablePacketPtr & sendPacket );

      // Receives data and parse it into packets
      void recvLoop ( );
      // Transforms packets into blob data and sends it
      void sendLoop ( );
      // Executes received packet actions
      void execLoop ( );

      void close ( );
      void handleCrash ( );

      template < class T >
      void registerReceivablePacket ( )
      {
        _packetHandler.registerPacketFactory < T > ( );
      }

    protected:
      virtual void initializeConnection ( ) = 0;

      std::string _connectAddress;
      uint16_t _connectPort;
      bool _active;
      ConnectionPtr _connection;

    private:
      std::mutex _sendMtx;
      std::condition_variable _sendMonitor;

      std::mutex _execMtx;
      std::condition_variable _execMonitor;
      std::queue < ReceivablePacketPtr > _execQueue;

      std::thread _recvThread;
      std::thread _sendThread;
      std::thread _executorThread;

      ByteBuffer _sendBuffer;
      ByteBuffer _recvBuffer;

      PacketHandler _packetHandler;
  };

  class RawClient : public Client
  {
    public:
      RawClient ( const std::string & address_, std::uint16_t port_ );
    protected:
      void initializeConnection ( );
  };

  class SecureClient : public Client
  {
    public:
      SecureClient ( const std::string & address_, std::uint16_t port_,
                     const std::string & keyFile_ = "",
                     const std::string & certFile_ = "",
                     const std::string & caFile_ = "" );
    protected:
      void initializeConnection ( );
    private:
      std::string _keyFile;
      std::string _certFile;
      std::string _caFile;
  };
}

#endif