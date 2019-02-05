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

  } streamingSessionInfo;

  // Key: session name, Value: session info
  typedef std::unordered_map < std::string, StreamingSessionInfo > sessionList;

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
                                               StreamingSessionInfo * result_ );

      void createSession ( User & user_, const std::string & sessionName_, int status_ );

      void tryFinishSession ( User & user_, const std::string & sessionName_ );

      void finishSession ( User & user_,
                           const std::string & sessionName_ );

      StreamingSessionInfo * getSessionInfo ( User & user_,
                                              const std::string & sessionName_ );

      const std::unordered_map < std::string, StreamingSessionInfo > & getUserSessions ( User & user_ );

    private:
      static SessionTracker _INSTANCE;

      // Key: user name, Value sessionList
      std::unordered_map < std::string, sessionList > _openSessions;
  };
}

#endif