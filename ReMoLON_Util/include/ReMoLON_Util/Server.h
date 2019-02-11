#ifndef __REMOLON_UTIL_SERVER_H__
#define __REMOLON_UTIL_SERVER_H__

#include <thread>
#include <vector>
#include <list>
#include <memory>
#include <string>

#include <Poco/Net/SecureServerSocket.h>

#include "ReMoLON_Util/SelectorThread.h"
#include "ReMoLON_Util/PacketHandler.h"
#include "ReMoLON_Util/ThreadPool.h"
#include "ReMoLON_Util/Config.h"

namespace remolonUtil
{
  class AbstractServer
  {
    public:
      AbstractServer ( const std::string & address_, std::uint16_t port_ );
      ~AbstractServer ( );

      void start ( bool blocking_ = false );

      virtual std::unique_ptr < Connection > createNewConnection ( const Poco::Net::Socket & socket_ ) = 0;
      
      void acceptLoop ( );

      void shutDown ( );

      template < class T >
      void registerReceivablePacket ( )
      {
        _packetHandler.registerPacketFactory < T > ( );
      }
    
    protected:
      virtual void initializeSocket ( ) = 0;

      std::unique_ptr < Poco::Net::ServerSocket > _serverSocket;

      std::string _bindIPAddress;
      std::uint16_t _bindPort;

    private:
      void assignConnectionToWorker ( ConnectionPtr & con_ );

      bool _active;
      bool _blocking;

      std::thread _acceptorThread;
      std::vector < SelectorThreadPtr > _selectorWorkers;

      PacketHandler _packetHandler;
      ThreadPool < ReceivablePacket > _packetThreadPool;
  };

  class RawServer : public AbstractServer
  {
    public:
      RawServer ( const std::string & address_, std::uint16_t port_ );
      std::unique_ptr < Connection > createNewConnection ( const Poco::Net::Socket & socket_ );
    protected:
      void initializeSocket ( );
  };

  class SecureServer : public AbstractServer 
  {
    public:
      SecureServer ( const std::string & address_, std::uint16_t port_,
                     const std::string & keyFile_,
                     const std::string & certFile_,
                     const std::string & caFile_ );

      std::unique_ptr < Connection > createNewConnection ( const Poco::Net::Socket & socket_ );
    protected:
      void initializeSocket ( );
    private:
      std::string _keyFilePath;
      std::string _certFilePath;
      std::string _caFilePath;
  };
}

#endif