#ifndef __REMOLON_FRONTEND_WEBSERVER_H__
#define __REMOLON_FRONTEND_WEBSERVER_H__

#include <vector>
#include <string>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

namespace remolonFrontend
{
	class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
	{
	  public:
		  virtual Poco::Net::HTTPRequestHandler * createRequestHandler ( const Poco::Net::HTTPServerRequest & );
	};

	class WebServer : public Poco::Util::ServerApplication
	{
	public:
		WebServer ( unsigned int port_ );
  private:
		unsigned int _port;
	protected:
		int main ( const std::vector < std::string > & );
	};
}

#endif
