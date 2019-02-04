#ifndef __REMOLON_FRONTEND_HTTPACTIONS_CREATESESSIONACTIONHANDLER_H__
#define __REMOLON_FRONTEND_HTTPACTIONS_CREATESESSIONACTIONHANDLER_H__

#include "AbstractActionHandler.h"

namespace remolonFrontend
{
	namespace httpactions
	{
		class CreateSessionActionHandler : public AbstractActionHandler
		{
			public:
				std::string getAction ( );
				void handleAction ( Poco::Net::HTTPServerRequest & request_, 
														Poco::Net::HTTPServerResponse & response_ );
		};
	}
}

#endif
