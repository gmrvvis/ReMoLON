#ifndef __REMOLON_FRONTEND_ABSTRACTACTIONHANDLER_H__
#define __REMOLON_FRONTEND_ABSTRACTACTIONHANDLER_H__

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <memory>

namespace remolonFrontend
{
	class AbstractActionHandler
	{
		public:
			virtual std::string getAction ( ) = 0;
			virtual void handleAction ( Poco::Net::HTTPServerRequest & request_, 
                                  Poco::Net::HTTPServerResponse & response_ ) = 0;
	};

	typedef std::unique_ptr< AbstractActionHandler > AbstractActionHandlerPtr;
}

#endif
