#ifndef __RRFRONTEND_SESSIONLISTACTIONHANDLER_H__
#define __RRFRONTEND_SESSIONLISTACTIONHANDLER_H__

#include "AbstractActionHandler.h"

namespace rrfrontend
{
	class SessionListActionHandler : public AbstractActionHandler
	{
		public:
			std::string getAction();
			void handleAction(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response);
	};
}

#endif
