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
 
#ifndef __REMOLON_FRONTEND_SESSIONTRACKER_H__
#define __REMOLON_FRONTEND_SESSIONTRACKER_H__

#include <string>
#include <unordered_map>
#include <vector>

#include "User.h"

namespace remolonFrontend
{
  enum class SessionStatus : unsigned char
  {
    SESSION_UNINITIALIZED = 0,
    SESSION_CREATING = 1,
    SESSION_RUNNING = 2,
    SESSION_CLOSING = 3,
    SESSION_CRASHED = 4
  };

  typedef struct StreamingSessionInfo
  {
    std::string _sessionName;
    std::string _ownerIp;
    std::string _ownerUsername;
    std::string _nodeAddress;
    std::string _nodePublicAddress;
    unsigned int _nodePort;
    SessionStatus _status;

    StreamingSessionInfo ( )
     : _status ( SessionStatus::SESSION_UNINITIALIZED )
    {
    }

  } TStreamingSessionInfo;

  // Key: session name, Value: session info
  typedef std::unordered_map < std::string, TStreamingSessionInfo > sessionList;

  enum class SessionCreationResult : unsigned char
  {
    CREATION_OK = 0,
    CREATION_FAIL_DUPLICATE_NAME = 1,
    CREATION_FAIL_NO_AVAILABLE_NODES = 2
  };

  class SessionTracker
  {
    public:
      static SessionTracker & getInstance ( );

      SessionCreationResult tryCreateSession ( User & usr_, 
                                               const std::string & sessionName_, 
                                               TStreamingSessionInfo * result_ );

      void createSession ( User & user_, const std::string & sessionName_, int status_ );

      void tryFinishSession ( User & user_, const std::string & sessionName_ );

      void finishSession ( User & user_,
                           const std::string & sessionName_ );

      TStreamingSessionInfo * getSessionInfo ( User & user_,
                                              const std::string & sessionName_ );

      const std::unordered_map < std::string, TStreamingSessionInfo > & getUserSessions ( User & user_ );

    private:
      static SessionTracker _INSTANCE;

      // Key: user name, Value sessionList
      std::unordered_map < std::string, sessionList > _openSessions;
  };
}

#endif