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
 
#ifndef __REMOLON_UTIL_CONNECTION_H__
#define __REMOLON_UTIL_CONNECTION_H__

#include <Poco/Net/StreamSocket.h>
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
  class Connection
  {
    public:
      void setPollSetSource ( Poco::Net::PollSet * pollSetSource_ );
      void sendPacket ( SendablePacketPtr & sendable_ );
      SendablePacketPtr pollSendablePacket ( );
      void endConnection ( );

      int getSockFD ( void );
      Poco::Net::StreamSocket * getSocket ( );

      std::string localAddress ( );
      std::string clientAddress ( );

    protected:
      Connection ( );

      std::unique_ptr < Poco::Net::StreamSocket > _socket;
    private:
      std::queue < SendablePacketPtr > _sendPackets;
      std::mutex _sendMtx;
      Poco::Net::PollSet * _owningPollset;
  };

  typedef std::unique_ptr < Connection > ConnectionPtr;

  class RawConnection : public Connection
  {
    public:
      RawConnection ( const std::string & address_, uint16_t port_ );
      RawConnection ( const Poco::Net::Socket & socket_ );
  };

  class SecureConnection : public Connection
  {
    public:
      SecureConnection ( const std::string & address_, uint16_t port_,
                         const std::string & keyFile_,
                         const std::string & certFile_,
                         const std::string & caFile_ );
      SecureConnection ( const Poco::Net::Socket & socket_ );
  };
}

#endif