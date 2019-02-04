#ifndef __REMOLON_UTIL_CLIENT_H__
#define __REMOLON_UTIL_CLIENT_H__

#include "ReMoLON_Util/SecureClient.h"

namespace remolonUtil
{
  class Client : public remolonUtil::SecureClient
  {
    public:
      Client ( const TClientConfig & config_ );
      void initializeSocket ( );
  };
}

#endif