#include "WebServer.h"

#include "WebServerRequestHandler.h"

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>

#include <iostream>

#include "RequestManager.h"

#include "DefaultActionHandler.h"
#include "LogInActionHandler.h"
#include "SessionListActionHandler.h"
#include "CreateSessionActionHandler.h"

namespace rrfrontend
{
	Poco::Net::HTTPRequestHandler * RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &)
	{
		return new WebServerRequestHandler();
	}

	// ======================================================

  int WebServer::main(const std::vector<std::string> &)
  {
		RequestManager::getInstance().registerActionHandler<DefaultActionHandler>();
		RequestManager::getInstance().registerActionHandler<LogInActionHandler>();
		RequestManager::getInstance().registerActionHandler<SessionListActionHandler>();
		RequestManager::getInstance().registerActionHandler<CreateSessionActionHandler>();

    Poco::Net::HTTPServer s(new RequestHandlerFactory(), Poco::Net::ServerSocket(7777), new Poco::Net::HTTPServerParams);

    s.start();
    std::cout << std::endl << "RR Frontend Server started" << std::endl;

    waitForTerminationRequest();  // wait for CTRL-C or kill

    std::cout << std::endl << "RR Frontend Server Shutting down..." << std::endl;
    s.stop();

    return Poco::Util::Application::EXIT_OK;
  }
}
