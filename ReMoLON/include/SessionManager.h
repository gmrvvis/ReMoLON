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
 
#ifndef __REMOLON_SESSIONMANAGER_H__
#define __REMOLON_SESSIONMANAGER_H__

#include <unordered_map>
#include <mutex>

#include "StreamingSession.h"

#define SESSION_LAUNCH_OK 0
#define SESSION_LAUNCH_DUPLICATE_NAME 1
#define SESSION_LAUNCH_PORT_UNAVAILABLE 2
#define SESSION_LAUNCH_CRASHED 3

namespace remolon
{
  typedef std::unordered_map < std::string, StreamingSession > sessionList;

  class SessionManager
  {
    public:
      static SessionManager & getInstance ( );

      void setPublicAddress ( const std::string & address_ );
      void addAvailablePort ( uint16_t port_ );
      void addAvailableSocketPort ( uint16_t port_ );
      void addAvailableRTCPort ( uint16_t port_ );

      void startNewSession ( const std::string & ownerName_,
                             const std::string & ownerAddress_,
                             const std::string & sessionName_,
                             uint16_t port_ );

      void finishSession ( const std::string & ownerName_,
                           const std::string & sessionName_ );
      void clearSession ( StreamingSession & session );

      void finishAllSessions ( );

      StreamingSession * getStreamSession ( const std::string & owernName_,
                                            const std::string & sessionName_ );

      const std::string & getPublicAddress ( );
      const std::unordered_map < uint16_t, bool > & getPortList ( );
      const std::unordered_map < uint16_t, bool > & getSocketPortList ( );
      const std::unordered_map < uint16_t, bool > & getRTCPortList ( );

    private:
      uint16_t findAvailablePort ( std::unordered_map < uint16_t, bool> & source_ );

      static SessionManager _INSTANCE;

      std::string _publicAddress;
      std::mutex _mtx;
      std::unordered_map < uint16_t, bool > _usedPorts;
      std::unordered_map < uint16_t, bool > _usedSockPorts;
      std::unordered_map < uint16_t, bool > _usedRTCPorts;
      std::unordered_map < std::string, sessionList > _streamSessions;  
  };
}

#endif