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