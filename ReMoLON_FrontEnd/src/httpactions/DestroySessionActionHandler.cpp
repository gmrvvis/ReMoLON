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
 
#include "httpactions/DestroySessionActionHandler.h"

#include <fstream>

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
    std::string DestroySessionActionHandler::getAction ( )
    {
      return "DestroySession";
    }

    void DestroySessionActionHandler::handleAction ( Poco::Net::HTTPServerRequest & request_, 
														                         Poco::Net::HTTPServerResponse & response_ )
    {
      const Poco::Net::SocketAddress & sAddr = request_.clientAddress ( );
			Poco::Net::IPAddress ipAddr = sAddr.host ( );
			std::string ipAddrStr = ipAddr.toString ( );

			response_.setStatus ( Poco::Net::HTTPResponse::HTTP_OK );

			if( !UserManager::getInstance ( ).isUserLoggedIn ( ipAddrStr ) )
			{
				std::ifstream file ( "FEClient/auth.html", std::ios::binary );
				response_.setContentType ( "text/html" );
				Poco::StreamCopier::copyStream ( file, response_.send ( ) );
			}
			else
			{
        User * usr = UserManager::getInstance ( ).getUser ( ipAddrStr );

				Poco::Net::HTMLForm form (request_, request_.stream ( ) );
				std::string sessionName = form.get ( "id", "" );

        SessionTracker::getInstance ( ).tryFinishSession ( *usr, sessionName );
      }
    }
  }
}
