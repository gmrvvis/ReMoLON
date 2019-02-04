#include "CreateSessionActionHandler.h"

#include <fstream>
#include <iostream>

#include <Poco/Net/HTMLForm.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/IPAddress.h>

#include "UserManager.h"
#include "SessionTracker.h"

namespace remolonFrontend
{
	namespace httpactions
	{
		std::string CreateSessionActionHandler::getAction ( )
		{
			return "CreateSession";
		}

		void CreateSessionActionHandler::handleAction ( Poco::Net::HTTPServerRequest & request_, 
																										Poco::Net::HTTPServerResponse & response_ )
		{
			const Poco::Net::SocketAddress & sAddr = request_.clientAddress ( );
			Poco::Net::IPAddress ipAddr = sAddr.host ( );
			std::string ipAddrStr = ipAddr.toString ( );

			response_.setStatus ( Poco::Net::HTTPResponse::HTTP_OK );

			if( !UserManager::getInstance ( ).isUserLoggedIn ( ipAddrStr ) )
			{
				std::ifstream file ( "client/auth.html", std::ios::binary );
				response_.setContentType ( "text/html" );
				Poco::StreamCopier::copyStream ( file, response_.send ( ) );
			}
			else
			{
				Poco::Net::HTMLForm form (request_, request_.stream ( ) );
				std::string newSessionName = form.get ( "session_name", "" );

				if ( newSessionName == "" )
				{
					response_.setStatus ( Poco::Net::HTTPResponse::HTTP_BAD_REQUEST );
					response_.send ( ) << "No session name specified";
				}
				else
				{
					User * usr = UserManager::getInstance ( ).getUser ( ipAddrStr );

					if ( usr == nullptr )
					{
						response_.setStatus ( Poco::Net::HTTPResponse::HTTP_FORBIDDEN );
						response_.send ( ) << "Unauthorized access from non identified used";
						return;
					}

          std::cout << "user " << usr->getUserName ( ) << " requested to create session with name " << newSessionName << std::endl;

					streamingSessionInfo info;

					SessionCreationResult 
					result = SessionTracker::getInstance ( ).tryCreateSession ( *usr, newSessionName, &info );

					switch ( result )
					{
						case SessionCreationResult::CREATION_FAIL_NO_AVAILABLE_NODES:
							response_.setStatus ( Poco::Net::HTTPResponse::HTTP_BAD_REQUEST );
							response_.send ( ) << "There are not available resources to create a new streaming session. Try again later.";
							break;
						case SessionCreationResult::CREATION_FAIL_DUPLICATE_NAME:
							response_.setStatus ( Poco::Net::HTTPResponse::HTTP_BAD_REQUEST );
							response_.send ( ) << "A session with the name " + newSessionName + " already exists.";
							break;
						case SessionCreationResult::CREATION_OK:
							response_.send ( ) << "";
							break;
					}
				}
			}
		}
	}
}
