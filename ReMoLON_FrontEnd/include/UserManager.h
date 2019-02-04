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
