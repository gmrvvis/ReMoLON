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

#include "WebServer.h"

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>

#include <iostream>

#include "WebServerRequestHandler.h"
#include "RequestManager.h"
#include "SessionManagerServer.h"
#include "httpactions/DefaultActionHandler.h"
#include "httpactions/LogInActionHandler.h"
#include "httpactions/SessionListActionHandler.h"
#include "httpactions/CreateSessionActionHandler.h"
#include "httpactions/DestroySessionActionHandler.h"

namespace remolonFrontend
{
	Poco::Net::HTTPRequestHandler * 
  RequestHandlerFactory::createRequestHandler ( const Poco::Net::HTTPServerRequest & )
	{
		return new WebServerRequestHandler();
	}

	// ======================================================

	WebServer::WebServer ( unsigned int port_ )
		: Poco::Util::ServerApplication ( )
		,	_port ( port_ )
	{
	}

  int WebServer::main ( const std::vector<std::string> & )
  {
		RequestManager::getInstance ( ).registerActionHandler < httpactions::DefaultActionHandler > ( );
		RequestManager::getInstance ( ).registerActionHandler < httpactions::LogInActionHandler > ( );
		RequestManager::getInstance ( ).registerActionHandler < httpactions::SessionListActionHandler > ( );
		RequestManager::getInstance ( ).registerActionHandler < httpactions::CreateSessionActionHandler > ( );
    RequestManager::getInstance ( ).registerActionHandler < httpactions::DestroySessionActionHandler > ( );

    Poco::Net::HTTPServer s ( new RequestHandlerFactory ( ), 
                              Poco::Net::ServerSocket ( _port ), 
                              new Poco::Net::HTTPServerParams );

    s.start ( );

    std::cout << "ReMoLON Frontend Server started" << std::endl;

    waitForTerminationRequest ( );  // wait for CTRL-C or kill

    //SessionManagerServer::getInstance ( ).shutDown ( );

    std::cout << "ReMoLON Frontend Server Shutting down..." << std::endl;
    s.stop ( );

    return Poco::Util::Application::EXIT_OK;
  }
}
