#ifndef __REMOLON_FRONTEND_USER_H__
#define __REMOLON_FRONTEND_USER_H__

#include <string>
#include <unordered_map>
#include <mutex>

namespace remolonFrontend
{
  class User
  {
    public:
      User ( );

      void setUnixUser ( const std::string & unixUser_ );
      void setPwd ( const std::string & pwd_ );
      void setAddress ( const std::string & address_ );
      void setUnixIds ( const uid_t & userID_, const gid_t & groupID_ );

      const std::string & getAddress ( );
      const std::string & getUserName ( );
      const uid_t & getUID ( );
      const gid_t & getGID ( );

      const std::unordered_map < std::string, std::string > & getActiveSessions ( );

      void addSession ( const std::string & sessionName_, 
                        const std::string & nodeAddress_ );

      void removeSession ( const std::string & sessionName_ );

      std::mutex & getUserLock ( );

    private:
      std::mutex _userLock;

      std::string _unixUser;
      std::string _unixPwd;
      std::string _ipAddress;
      uid_t _userID;
      gid_t _groupID;

      std::unordered_map < std::string, std::string > _activeSessions;
  };
}

#endif