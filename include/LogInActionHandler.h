#ifndef __RRFRONTEND_LOGINACTIONHANDLER_H__
#define __RRFRONTEND_LOGINACTIONHANDLER_H__

#include "AbstractActionHandler.h"

namespace rrfrontend
{
	class LogInActionHandler : public AbstractActionHandler
	{
		public:
			std::string getAction();
			void handleAction(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response);
	};
}

#endif
