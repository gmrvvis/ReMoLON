#ifndef __REMOLON_REMOTOOSERVERPACKETS_REQUESTSESSIONINFO_H__
#define __REMOLON_REMOTOOSERVERPACKETS_REQUESTSESSIONINFO_H__

#include "ReMoLON_Util/Packet.h"

namespace remolon
{
  namespace remotooserverpackets
  {
    class RequestSessionInfo : public remolonUtil::SendablePacket
    {
      public:
        char getOpcode ( );
        void writeImpl ( );
    };
  }
}

#endif