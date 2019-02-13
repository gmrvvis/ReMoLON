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
 
#ifndef __REMOLON_FRONTEND_USERMANAGER_H__
#define __REMOLON_FRONTEND_USERMANAGER_H__

#include <mutex>
#include <unordered_map>
#include <vector>
#include <string>
#include <sys/types.h>

#include "User.h"

namespace remolonFrontend
{
	class UserManager
	{
		public:
			static UserManager & getInstance ( );
		
			~UserManager ( );
	
			bool isUserLoggedIn ( const std::string & ipAddress_ );
	
			bool tryAuthUser ( const std::string & ipAddress_, 
                         const std::string & user_, 
                         const std::string & pwd_ );

			User * getUser ( const std::string & ipAddress_ );
			User * getUserByName ( const std::string & userName_ );

			bool logOutUser ( const std::string & ipAddress_ );

		private:
			static UserManager _INSTANCE;

      UserManager ( );
			bool authenticate ( const char * service_, 
                          char * user_, 
                          char * pass_ );

      std::unordered_map < std::string, User > _loggedInUsers;
			std::mutex _mtx;
	};
}

#endif
