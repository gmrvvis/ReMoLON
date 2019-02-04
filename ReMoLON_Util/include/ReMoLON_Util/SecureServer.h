#ifndef __REMOLON_UTIL_SECURESERVER_H__
#define __REMOLON_UTIL_SECURESERVER_H__

#include <thread>
#include <vector>
#include <memory>
#include <string>

#include <Poco/Net/SecureServerSocket.h>

#include "ReMoLON_Util/SelectorThread.h"
#include "ReMoLON_Util/PacketHandler.h"
#include "ReMoLON_Util/ThreadPool.h"
#include "ReMoLON_Util/Config.h"

namespace remolonUtil
{
  typedef struct ServerConfig
  {
    std::string _bindToAddress;
    uint16_t _bindToPort;
    std::string _keyFilePath;
    std::string _certFilePath;
    std::string _caFilePath;
  } TServerConfig;

  class SecureServer
  {
    public:
      SecureServer ( const TServerConfig & serverConfig_ );
                
      ~SecureServer ( );

      void start ( );
                
      void acceptLoop ( );

      void shutDown ( );

      template < class T >
      void registerReceivablePacket ( )
      {
        _packetHandler.registerPacketFactory < T > ( );
      }
    
    protected:
      virtual void initializeSocket ( );

      std::unique_ptr < Poco::Net::SecureServerSocket > _serverSocket;

      std::string _bindIPAddress;
      uint16_t _bindPort;

    private:
      void assignConnectionToWorker ( Connection & con_ );

      std::string _keyFilePath;
      std::string _certFilePath;
      std::string _caFilePath;

      bool _active;

      std::thread _acceptorThread;
      std::vector < SelectorThreadPtr > _selectorWorkers;

      PacketHandler _packetHandler;
      ThreadPool < ReceivablePacket > _packetThreadPool;
  };
}

#endif