#ifndef __REMOLON_FRONTEND_HTTPACTIONS_LOGINACTIONHANDLER_H__
#define __REMOLON_FRONTEND_HTTPACTIONS_LOGINACTIONHANDLER_H__

#include "AbstractActionHandler.h"

namespace remolonFrontend
{
	namespace httpactions
	{
		class LogInActionHandler : public AbstractActionHandler
		{
			public:
				std::string getAction ( );
				void handleAction( Poco::Net::HTTPServerRequest & request_, 
													 Poco::Net::HTTPServerResponse & response_ );
		};
	}
}

#endif
