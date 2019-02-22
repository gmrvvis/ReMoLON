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
      virtual ~Client ( void );
      
      void connect ( );
      void sendPacket ( SendablePacketPtr & sendPacket );

      // Receives data and parse it into packets
      void recvLoop ( );
      // Transforms packets into blob data and sends it
      void sendLoop ( );
      // Executes received packet actions
      void execLoop ( );

      virtual void onConnectionClose ( );
      void close ( );
      void handleCrash ( );

      template < class T >
      void registerReceivablePacket ( )
      {
        _packetHandler.registerPacketFactory < T > ( );
      }

    protected:
      virtual void initializeConnection ( ) = 0;

      bool _active;
      std::string _connectAddress;
      uint16_t _connectPort;
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
      void onConnectionClose ( );
    protected:
      void initializeConnection ( );
    private:
      std::string _keyFile;
      std::string _certFile;
      std::string _caFile;
  };
}

#endif
