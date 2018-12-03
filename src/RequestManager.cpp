#include "RequestManager.h"

#include <iostream>

namespace rrfrontend
{
	RequestManager RequestManager::INSTANCE;

	RequestManager & RequestManager::getInstance()
	{
		return INSTANCE;
	}

	RequestManager::RequestManager()
	{
	}

	RequestManager::~RequestManager()
	{
	}

	void RequestManager::handleRequest(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response)
	{
		std::string action = request.getURI();

		if(action.find_first_of("/") == 0)
		{
			action = action.substr(1, action.length());
		}

		auto it = actionHandlers.find(action);

		if(it == actionHandlers.end())
		{
			std::cout << "RequestHandler: No handler for action: "  << action << std::endl;
			response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
		}	
		else
		{
			std::cout << "Handling request for ACTION = " << action << std::endl;
			AbstractActionHandlerPtr & handler = it->second;
			handler.get()->handleAction(request, response);
		}
	}
}	
