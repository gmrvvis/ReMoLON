#include "RequestManager.h"

#include <iostream>

namespace remolonFrontend
{
	RequestManager RequestManager::_INSTANCE;

	RequestManager & RequestManager::getInstance ( )
	{
		return _INSTANCE;
	}

	RequestManager::RequestManager ( )
	{
	}

	RequestManager::~RequestManager ( )
	{
	}

	void RequestManager::handleRequest ( Poco::Net::HTTPServerRequest & request_, 
                                       Poco::Net::HTTPServerResponse & response_ )
	{
		std::string action = request_.getURI ( );

		if ( action.find_first_of ( "/" ) == 0 )
		{
			action = action.substr ( 1, action.length ( ) );
		}

		auto it = _actionHandlers.find ( action );

		if ( it == _actionHandlers.end ( ) )
		{
			std::cout << "RequestHandler: No handler for action: "  << action << std::endl;
			response_.setStatus ( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
		}	
		else
		{
			std::cout << "Handling request for ACTION = " << action << std::endl;
			AbstractActionHandlerPtr & handler = it->second;
			handler.get ( )->handleAction ( request_, response_ );
		}
	}
}	
