#ifndef __REMOLON_UTIL_SERVER_H__
#define __REMOLON_UTIL_SERVER_H__

#include "ReMoLON_Util/SecureServer.h"

namespace remolonUtil
{
  class Server : public SecureServer
  {
    public:
      Server ( const TServerConfig & serverConfig_ );
    
    protected:
      void initializeSocket ( );
  };
}

#endif