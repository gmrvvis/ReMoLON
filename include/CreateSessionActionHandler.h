#ifndef __RRFRONTEND_CREATESESSIONACTIONHANDLER_H__
#define __RRFRONTEND_CREATESESSIONACTIONHANDLER_H__

#include "AbstractActionHandler.h"

namespace rrfrontend
{
	class CreateSessionActionHandler : public AbstractActionHandler
	{
		public:
			std::string getAction();
			void handleAction(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response);
	};
}

#endif
