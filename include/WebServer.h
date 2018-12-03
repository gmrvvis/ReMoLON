#ifndef __RRFRONTEND_WEBSERVER_H__
#define __RRFRONTEND_WEBSERVER_H__

#include <vector>
#include <string>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

namespace rrfrontend
{
	class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		virtual Poco::Net::HTTPRequestHandler * createRequestHandler(const Poco::Net::HTTPServerRequest &);
	};

	class WebServer : public Poco::Util::ServerApplication
	{
	protected:
		int main(const std::vector<std::string> &);
	};
}

#endif
