#ifndef __RRFRONTEND_DEFAULTACTIONHANDLER_H__
#define __RRFRONTEND_DEFAULTACTIONHANDLER_H__

#include "AbstractActionHandler.h"

namespace rrfrontend
{
	class DefaultActionHandler : public AbstractActionHandler
	{
		public:
			std::string getAction();
			void handleAction(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response);
	};
}

#endif
