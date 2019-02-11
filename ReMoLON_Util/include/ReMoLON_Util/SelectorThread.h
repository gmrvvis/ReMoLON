#ifndef __REMOLON_UTIL_SELECTORTHREAD_H__
#define __REMOLON_UTIL_SELECTORTHREAD_H__

#include <Poco/Net/PollSet.h>

#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>

#include "ReMoLON_Util/Connection.h"
#include "ReMoLON_Util/ByteBuffer.h"
#include "ReMoLON_Util/PacketHandler.h"
#include "ReMoLON_Util/ThreadPool.h"


namespace remolonUtil
{
  class SelectorThread
  {
    public:
      SelectorThread ( ThreadPool < ReceivablePacket > & tpool_,
                       PacketHandler & pHandler_ );
      ~SelectorThread ( );

      std::size_t getNumConnections ( );

      bool addConnection ( ConnectionPtr & con_ );
      bool removeConnection ( Connection * con_ );

      void selectLoop ( );

      void shutDown ( );

    private:
      void readFromConnection ( Connection * con_ );
      void writeToConnection ( Connection * con_ );
      void handleConnectionCrash ( Connection * con_ );

      ByteBuffer _readBuffer;
      ByteBuffer _writeBuffer;
      ByteBuffer _writeBufferHelper;

      std::thread _worker;

      bool _active;

      std::map < int, ConnectionPtr > _connections;
      Poco::Net::PollSet _socketProcessSet;

      std::mutex _mtx;
      std::condition_variable _monitor;

      PacketHandler * _packetHandler;
      ThreadPool < ReceivablePacket > * _threadPool;
  };

  typedef std::unique_ptr < SelectorThread > SelectorThreadPtr;
}

#endif