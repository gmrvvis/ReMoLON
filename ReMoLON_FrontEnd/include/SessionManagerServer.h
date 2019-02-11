#ifndef __REMOLON_FRONTEND_INSTANCESERVER_H__
#define __REMOLON_FRONTEND_INSTANCESERVER_H__

#include <memory>
#include <thread>
#include <vector>

#include "ReMoLON_Util/Server.h"

#define REMOLON_NO_SSL_

namespace remolonFrontend
{
	class SessionManagerServer
	{
		public:
      static SessionManagerServer & getInstance ( );

			~SessionManagerServer ( );

			void start ( );
			void shutDown ( );

    private:
      SessionManagerServer ( );

			static SessionManagerServer _INSTANCE;
      
#ifdef REMOLON_NO_SSL_
			std::unique_ptr < remolonUtil::RawServer > _server;
#else
      std::unique_ptr < remolonUtil::SecureServer > _server;
#endif
	};
}

#endif
