#ifndef __RRFRONTEND_ABSTRACTACTIONHANDLER_H__
#define __RRFRONTEND_ABSTRACTACTIONHANDLER_H__

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <memory>

namespace rrfrontend
{
	class AbstractActionHandler
	{
		public:
			virtual std::string getAction() = 0;
			virtual void handleAction(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response) = 0;
	};

	typedef std::unique_ptr<AbstractActionHandler> AbstractActionHandlerPtr;
}

#endif
