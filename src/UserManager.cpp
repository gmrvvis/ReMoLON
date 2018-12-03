#include "UserManager.h"

#include <security/pam_appl.h>
#include <cstring>
#include <pwd.h>

#include <iostream>

namespace rrfrontend
{
	struct pam_response *reply;

	int null_conv(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr) 
	{
		*resp = reply;
		return PAM_SUCCESS;
	}

	static struct pam_conv conv = { null_conv, NULL };

	UserManager UserManager::INSTANCE;

	UserManager & UserManager::getInstance()
	{
		return INSTANCE;
	}

	UserManager::UserManager() { }

	UserManager::~UserManager() { }
	
	bool UserManager::isUserLoggedIn(const std::string & ipAddress)
	{
		std::unique_lock<std::mutex> lock(mtx);
		bool result = _loggedInUsers.find(ipAddress) != _loggedInUsers.end();
		lock.unlock();
		return result;
	}

	bool UserManager::tryAuthUser(const std::string & ipAddress, const std::string & user, const std::string & pwd)
	{
		std::unique_lock<std::mutex> lock(mtx);

		bool result;
		char * usr_cstr = new char [user.length() + 1];
		std::strcpy (usr_cstr, user.c_str());

		char * pwd_cstr = new char [pwd.length() + 1];
		std::strcpy (pwd_cstr, pwd.c_str());

		if(authenticate("system-auth", usr_cstr, pwd_cstr))
		{
			UserData ud;
			ud.unixUser = user;
			ud.unixPwd = pwd;
			ud.ipAddress = ipAddress;

			struct passwd * userInfo = getpwnam(usr_cstr);

			if(!userInfo)
			{
				return false;
			}

			ud.userID = userInfo->pw_uid;
			ud.groupID = userInfo->pw_gid;

			_loggedInUsers[ipAddress] = ud;
			result = true;
		}
		else
		{
			result = false;
		}


		lock.unlock();
		return result;
	}

	bool UserManager::authenticate(const char * service, char * user, char * pass)
	{
		pam_handle_t *pamh = NULL;
		int retval = pam_start(service, user, &conv, &pamh);

		if (retval == PAM_SUCCESS) 
		{
			reply = (struct pam_response *)malloc(sizeof(struct pam_response));
			reply[0].resp = pass;
			reply[0].resp_retcode = 0;

			retval = pam_authenticate(pamh, 0);
			/*
			if (retval == PAM_SUCCESS)
			{
				fprintf(stdout, "Authenticated\n");
			}
			else
			{
				fprintf(stdout, "Not Authenticated\n");
			}
			*/
			pam_end(pamh, PAM_SUCCESS);
		}

		return retval == PAM_SUCCESS;
	}

	UserData & UserManager::getUserData(const std::string & ipAddress)
	{
		return _loggedInUsers[ipAddress];
	}

	bool UserManager::logOutUser(const std::string & ipAddress)
	{
		std::unique_lock<std::mutex> lock(mtx);
		auto it = _loggedInUsers.find(ipAddress);
		bool result = it != _loggedInUsers.end();

		if(result)
		{
			_loggedInUsers.erase(it);
		}

		lock.unlock();

		return result;
	}
}
