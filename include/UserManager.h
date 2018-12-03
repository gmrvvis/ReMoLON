#ifndef __RRFRONTEND_USERMANAGER_H__
#define __RRFRONTEND_USERMANAGER_H__

#include <mutex>
#include <unordered_map>
#include <vector>
#include <string>
#include <sys/types.h>

namespace rrfrontend
{
	typedef struct UserData
	{
		std::string unixUser;
		std::string unixPwd;
		std::string ipAddress;
		std::string server;
		std::string port;
		uid_t userID;
		gid_t groupID;
		
	}	UserData;

	class UserManager
	{
		private:
			static UserManager INSTANCE;
		public:
			static UserManager & getInstance();
		private:
			std::unordered_map<std::string, UserData> _loggedInUsers;
			std::mutex mtx;
		private:
			UserManager();
		public:
			~UserManager();
	
			bool isUserLoggedIn(const std::string & ipAddress);
	
			bool tryAuthUser(const std::string & ipAddress, const std::string & user, const std::string & pwd);

			UserData & getUserData(const std::string & ipAddress);

			bool logOutUser(const std::string & ipAddress);

		private:
			bool authenticate(const char * service, char * user, char * pass);
	};
}

#endif
