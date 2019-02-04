#ifndef __REMOLON_FRONTEND_INSTANCESERVER_H__
#define __REMOLON_FRONTEND_INSTANCESERVER_H__

#include <memory>
#include <thread>
#include <vector>

#include "ReMoLON_Util/SecureServer.h"

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
      
      std::unique_ptr < remolonUtil::SecureServer > _server;
	};
}

#endif
