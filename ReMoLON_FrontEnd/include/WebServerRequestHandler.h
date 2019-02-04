#ifndef __REMOLON_FRONTEND_WEBSERVERREQUESTHANDLER_H__
#define __REMOLON_FRONTEND_WEBSERVERREQUESTHANDLER_H__

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <vector>
#include <thread>

namespace remolonFrontend
{
	class WebServerRequestHandler : public Poco::Net::HTTPRequestHandler
	{
	public:
		virtual void handleRequest ( Poco::Net::HTTPServerRequest & request_, 
                                 Poco::Net::HTTPServerResponse & response_ );
	};
}

#endif
