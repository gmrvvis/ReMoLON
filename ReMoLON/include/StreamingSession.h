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
 
#ifndef __REMOLON_STREAMINGSESSION_H__
#define __REMOLON_STREAMINGSESSION_H__

#include <unordered_map>
#include <string>
#include <mutex>
#include <thread>

#include "ReMoLON_Util/Connection.h"

namespace remolon
{
  class StreamingSession
  {
    public:
      StreamingSession ( );

      void setSessionName ( const std::string & sessionName_ );
      void setSessionOwner ( const std::string & sessionOwner_ );
      void setSessionOwnerAddress ( const std::string & ownerAddress_ );
      void setSessionPorts ( uint16_t port_, uint16_t sockPort_, uint16_t rtcPort_ );
      void setConnection ( remolonUtil::Connection * con_ );
      void addAllowedUser ( const std::string & userName_, const std::string & userAddress_ );

      const std::string & getSessionName ( );
      const std::string & getSessionOwner ( );
      const std::string & getSessionOwnerAddress ( );
      uint16_t getSessionPort ( );
      uint16_t getSessionSocketPort ( );
      uint16_t getSessionRTCPort ( );

      bool tryLaunchSession ( );
      void finishSession ( );

    private:
      std::string _sessionName;
      std::string _sessionOwner;
      std::string _sessionOwnerAddress;
      uint16_t _sessionPort;
      uint16_t _sessionSockPort;
      uint16_t _sessionRTCPort;

      std::thread _streamingProcess;

      std::unordered_map < std::string, std::string > _allowedUsers;

      remolonUtil::Connection * _connection;

      std::mutex _mtx;
  };
}

#endif