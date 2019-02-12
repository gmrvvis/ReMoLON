#include "WebServer.h"

#include "WebServerRequestHandler.h"

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>

#include <iostream>

#include "RequestManager.h"

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

    std::cout << "ReMoLON Frontend Server Shutting down..." << std::endl;
    s.stop ( );

    return Poco::Util::Application::EXIT_OK;
  }
}
