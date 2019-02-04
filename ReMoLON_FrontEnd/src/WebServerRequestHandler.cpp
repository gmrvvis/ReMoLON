#include "WebServerRequestHandler.h"

#include "RequestManager.h"

namespace remolonFrontend
{
	void WebServerRequestHandler::handleRequest ( Poco::Net::HTTPServerRequest & request_, 
                                                Poco::Net::HTTPServerResponse & response_ )
  {
		RequestManager::getInstance ( ).handleRequest ( request_, response_ );
  }
}
