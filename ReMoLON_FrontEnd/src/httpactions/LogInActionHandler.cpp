#include "LogInActionHandler.h"

#include <fstream>

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTMLForm.h>

#include "UserManager.h"

namespace remolonFrontend
{
	namespace httpactions
	{
		std::string LogInActionHandler::getAction ( )
		{
			return "authenticate";
		}

		void LogInActionHandler::handleAction ( Poco::Net::HTTPServerRequest & request_, 
																						Poco::Net::HTTPServerResponse & response_ )
		{
			const Poco::Net::SocketAddress & sAddr = request_.clientAddress ( );
			Poco::Net::IPAddress ipAddr = sAddr.host ( );
			std::string ipAddrStr = ipAddr.toString ( );

			response_.setStatus ( Poco::Net::HTTPResponse::HTTP_OK );

			Poco::Net::HTMLForm form (request_, request_.stream() );

			if( !form.has ( "uname" ) 
					|| !form.has ( "psw" )
					|| !UserManager::getInstance().tryAuthUser ( ipAddrStr, 
																											form.get ( "uname", "" ), 
																											form.get ( "psw", "" ) ) )
			{
				std::ifstream file ( "client/auth.html", std::ios::binary );
				response_.setContentType ( "text/html" );
				Poco::StreamCopier::copyStream ( file, response_.send ( ) );
			}
			else if ( UserManager::getInstance ( ).isUserLoggedIn ( ipAddrStr ) )
			{
				std::ifstream file ( "client/index.html", std::ios::binary );
				response_.setContentType ( "text/html" );
				Poco::StreamCopier::copyStream ( file, response_.send ( ) );
			}
		}
	}
}
