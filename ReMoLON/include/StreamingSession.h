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
      void setSessionPort ( uint16_t port_ );
      void setConnection ( remolonUtil::Connection * con_ );
      void addAllowedUser ( const std::string & userName_, const std::string & userAddress_ );

      const std::string & getSessionName ( );
      const std::string & getSessionOwner ( );
      uint16_t getSessionPort ( );

      bool tryLaunchSession ( );
      void finishSession ( );

    private:
      std::string _sessionName;
      std::string _sessionOwner;
      std::string _sessionOwnerAddress;
      uint16_t _sessionPort;

      std::thread _streamingProcess;

      std::unordered_map < std::string, std::string > _allowedUsers;

      remolonUtil::Connection * _connection;

      std::mutex _mtx;
  };
}

#endif