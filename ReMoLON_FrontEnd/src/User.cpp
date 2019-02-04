#include "User.h"

namespace remolonFrontend
{
  User::User ( )
  {

  }

  void User::setUnixUser ( const std::string & unixUser_ )
  {
    _unixUser = unixUser_;
  }

  void User::setPwd ( const std::string & pwd_ )
  {
    _unixPwd = pwd_;
  }

  void User::setAddress ( const std::string & address_ )
  {
    _ipAddress = address_;
  }

  void User::setUnixIds ( const uid_t & UserID_, const gid_t & groupID_ )
  {
    _userID = UserID_;
    _groupID = groupID_;
  }

  const std::string & User::getAddress ( )
  {
    return _ipAddress;
  }

  const std::string & User::getUserName ( )
  {
    return _unixUser;
  }

  const uid_t & User::getUID ( )
  {
    return _userID;
  }

  const gid_t & User::getGID ( )
  {
    return _groupID;
  }

  const std::unordered_map < std::string, std::string > & User::getActiveSessions ( )
  {
    return _activeSessions;
  }

  void User::addSession ( const std::string & sessionName_, 
                          const std::string & nodeAddress_ )
  {
    _activeSessions [ sessionName_ ] = nodeAddress_;
  }

  void User::removeSession ( const std::string & sessionName_ )
  {
    auto it = _activeSessions.find ( sessionName_ );
    if ( it != _activeSessions.end ( ) )
    {
      _activeSessions.erase ( it );
    }
  }

  std::mutex & User::getUserLock ( )
  {
    return _userLock;
  }
}