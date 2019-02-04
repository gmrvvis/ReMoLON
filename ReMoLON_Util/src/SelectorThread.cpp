#include "ReMoLON_Util/SelectorThread.h"

#include <string>
#include <iostream>
#include <chrono>

#include "ReMoLON_Util/PacketHandler.h"

namespace remolonUtil
{
  SelectorThread::SelectorThread ( ThreadPool < ReceivablePacket > & tPool_,
                                   PacketHandler & pHandler_ )
   : _active ( true )
  {
    _packetHandler = &pHandler_;
    _threadPool = &tPool_;

    _writeBufferHelper.resize ( 8192 );

    _worker = std::thread ( &SelectorThread::selectLoop, this );
  }

  SelectorThread::~SelectorThread ( )
  {
    shutDown ( );
  }

  std::size_t SelectorThread::getNumConnections ( )
  {
    std::unique_lock < std::mutex > lock ( _mtx );

    return _connections.size ( );
  }

  bool SelectorThread::addConnection ( Connection * con_ )
  {
    std::unique_lock < std::mutex > lock ( _mtx );

    // Only add it if we are not owning it already
    auto it =  _connections.find ( con_->getSockFD ( ) );
    if ( it == _connections.end ( ) )
    {
      // Set the connection polling source so it can auto-udpate itself
      con_->setPollSetSource ( &_socketProcessSet );
      // Add the  connection to the pollset and the tracking map
      _connections [ con_->getSockFD ( ) ] =  con_;
      _socketProcessSet.add ( *con_, Poco::Net::PollSet::POLL_READ );

      // Notify the loop thread in case it was sleeping
      // due lack of connections
      _monitor.notify_all ( );

      return true;
    }

    return false;
  }

  bool SelectorThread::removeConnection ( Connection * con_ )
  {
    auto it =  _connections.find ( con_->getSockFD ( ) );
    if ( it != _connections.end ( ) )
    {
      _socketProcessSet.remove ( *con_ );
      con_->close ( );
      _connections.erase ( it );

      return true;
    }

    return false;
  }

  void SelectorThread::selectLoop ( )
  {
    while ( _active )
    {
      // If no connections, sleep
      std::unique_lock < std::mutex > lock ( _mtx );
      while( _active && _connections.empty ( ) )
      {
        _monitor.wait ( lock );
      }

      // If we have connections, try to poll I/O ready sockets ( 50 milliseconds timeout )
      std::map< Poco::Net::Socket, int > polledSokets = _socketProcessSet.poll ( Poco::Timespan ( 50000 ) );

      // If we gather any ready socket process it
      if( polledSokets.size ( ) > 0 )
      {
        auto it = polledSokets.begin ( );
        for ( ; it != polledSokets.end ( ); it++ )
        {
          // POCO should implement a template-parametrized PollSet to retrieve
          // references with actual connection data on select/epoll, instead of having to perform
          // a map lookup using the socket's file descriptor

          try
          {
            int fd = it->first.impl ( )->sockfd ( );

            Connection * con = _connections [ fd ];

            int mask = it->second;
            if ( mask & Poco::Net::PollSet::POLL_READ )
            {
              readFromConnection ( con );
            }
            if ( mask & Poco::Net::PollSet::POLL_WRITE )
            {
              writeToConnection ( con );
            }
            if ( mask & Poco::Net::PollSet::POLL_ERROR )
            {
              handleConnectionCrash ( con );
            }
          }
          catch ( Poco::Exception & e )
          {
            std::cout << "Selector thread exception: " << e.displayText ( ) << std::endl;
          }
        }
      }
    }
  }

  void SelectorThread::readFromConnection ( Connection * con_ )
  {
    int total = 0;
    int received;
    int offset = 0;

    // Discard any previous content from another connection
    // TODO: Temporary-auxiliar buffers per client to save split content?
    // Shouldn't be necessary as the protocol is compossed of small packets
    _readBuffer.reset ( );

    try
    {
      while( (received = con_->receiveBytes( _readBuffer.getAppendPtr( ), 
                                            _readBuffer.getRemainingWriteSize ( ))) > 0 )
      {
        // Write  directly into the selector's read buffer and update the write pos
        // Instead of using an auxiliar array
        total += received;
        _readBuffer.setWritePos ( _readBuffer.getWritePos ( ) + received );

        try
        {
          if ( total > 0 )
          {
            ReceivablePacketPtr packet = _packetHandler->handlePacket ( _readBuffer, *con_ );
            if ( packet )
            {
              _threadPool->executeTask ( std::move ( packet ) );
            }
          }
          else
          {
            removeConnection ( con_ );
          }
        }
        catch ( std::exception & e )
        {
          std::cerr << "SelectorThread - error while reading from connection " << (con_->address ( ).toString ( )) << ": " << e.what ( ) << std::endl;
        }
      }
    } 
    catch ( Poco::TimeoutException & te )
    {
    }
  }

  void SelectorThread::writeToConnection ( Connection * con_ )
  {
    _writeBuffer.reset ( );

    SendablePacketPtr sendPtr;
    while( ( sendPtr = con_->pollSendablePacket ( ) ) != nullptr )
    {
      _writeBufferHelper.reset ( );
      SendablePacket * sp = sendPtr.get ( );
      // Stablish the buffer we will write to
      sp->setBuffer ( &_writeBufferHelper );
      sp->writePacket ( );

      // Try to append the packet data into the send buffer
      if ( !_writeBuffer.appendContent ( _writeBufferHelper ))
      {
        // If we were unable (the send buffer is full)
        // Send the data to the client, then reset the buffer and append the contents
        con_->sendBytes ( _writeBuffer.getReadPtr ( ), _writeBuffer.getUsedSize ( ), 0 );
        _writeBuffer.reset ( );
        _writeBuffer.appendContent ( _writeBufferHelper );
      }
    }

    // Send the remaining packets
    if( _writeBuffer.getUsedSize ( ) > 0)
    {
      con_->sendBytes ( _writeBuffer.getReadPtr ( ), _writeBuffer.getUsedSize ( ), 0 );
    }
  }

  void SelectorThread::handleConnectionCrash ( Connection * con_ )
  {
    std::cerr << "Connection from " << con_->address ( ).toString ( ) << " crashed!" << std::endl;
    removeConnection ( con_ );
  }

  void SelectorThread::shutDown ( )
  {
    _active = false;
    _monitor.notify_all ( );
    if ( _worker.joinable ( ) )
    {
      _worker.join ( );
    }
  }
}