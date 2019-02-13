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