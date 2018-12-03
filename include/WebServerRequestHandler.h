#ifndef __RRFRONTEND_WEBSERVERREQUESTHANDLER_H__
#define __RRFRONTEND_WEBSERVERREQUESTHANDLER_H__

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <vector>
#include <thread>

namespace rrfrontend
{
	class WebServerRequestHandler : public Poco::Net::HTTPRequestHandler
	{
	public:
		virtual void handleRequest(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response);
	};
}

#endif
