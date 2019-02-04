#include "UserManager.h"

#include <security/pam_appl.h>
#include <cstring>
#include <pwd.h>

#include <iostream>

namespace remolonFrontend
{
	struct pam_response *reply;

	int null_conv( int num_msg_, 
                 const struct pam_message ** msg_, 
                 struct pam_response ** resp_, 
                 void * appdata_ptr_ ) 
	{
		*resp_ = reply;
		return PAM_SUCCESS;
	}

	static struct pam_conv conv = { null_conv, NULL };

	UserManager UserManager::_INSTANCE;

	UserManager & UserManager::getInstance ( )
	{
		return _INSTANCE;
	}

	UserManager::UserManager ( ) 
  { 
  }

	UserManager::~UserManager ( ) 
  {
  }
	
	bool UserManager::isUserLoggedIn ( const std::string & ipAddress_ )
	{
		std::unique_lock < std::mutex > lock ( _mtx );
		bool result = _loggedInUsers.find ( ipAddress_ ) != _loggedInUsers.end ( );
		lock.unlock ( );
		return result;
	}

	bool UserManager::tryAuthUser ( const std::string & ipAddress_, 
                                  const std::string & user_, 
                                  const std::string & pwd_ )
	{
		std::unique_lock < std::mutex > lock ( _mtx );

		bool result;
		char * usr_cstr = new char [ user_.length ( ) + 1 ];
		std::strcpy ( usr_cstr, user_.c_str ( ) );

		char * pwd_cstr = new char [ pwd_.length ( ) + 1 ];
		std::strcpy ( pwd_cstr, pwd_.c_str ( ) );

		if ( authenticate( "system-auth", usr_cstr, pwd_cstr ) )
		{
			struct passwd * userInfo = getpwnam ( usr_cstr );

			if ( !userInfo )
			{
				return false;
			}

			User & usr = _loggedInUsers [ ipAddress_ ];
			usr.setUnixUser ( user_ );
			usr.setPwd ( pwd_ );
			usr.setAddress ( ipAddress_ );
			usr.setUnixIds ( userInfo->pw_uid, userInfo->pw_gid );

			result = true;
		}
		else
		{
			result = false;
		}

		lock.unlock ( );
		return result;
	}

	bool UserManager::authenticate ( const char * service_, char * user_, char * pass_ )
	{
		pam_handle_t *pamh = NULL;
		int retval = pam_start ( service_, user_, &conv, &pamh );

		if ( retval == PAM_SUCCESS ) 
		{
			reply = ( struct pam_response * ) malloc ( sizeof ( struct pam_response ) );
			reply [ 0 ].resp = pass_;
			reply [ 0 ].resp_retcode = 0;

			retval = pam_authenticate ( pamh, 0 );
			
			pam_end ( pamh, PAM_SUCCESS );
		}

		return retval == PAM_SUCCESS;
	}

	User * UserManager::getUser ( const std::string & ipAddress_ )
	{
		auto it = _loggedInUsers.find ( ipAddress_ );
		if ( it != _loggedInUsers.end ( ) )
		{
			return &(it->second);
		}

		return nullptr;
	}

	User * UserManager::getUserByName ( const std::string & userName_ )
	{
		auto it = _loggedInUsers.begin ( );
		for ( ; it != _loggedInUsers.end ( ); it++ )
		{
			if ( it->second.getUserName ( ) == userName_ )
			{
				return &(it->second);
			}
		}

		return nullptr;
	}

	bool UserManager::logOutUser ( const std::string & ipAddress_ )
	{
		std::unique_lock < std::mutex > lock ( _mtx );
		auto it = _loggedInUsers.find ( ipAddress_ );
		bool result = it != _loggedInUsers.end ( );

		if ( result )
		{
			_loggedInUsers.erase ( it );
		}

		lock.unlock ( );

		return result;
	}
}
