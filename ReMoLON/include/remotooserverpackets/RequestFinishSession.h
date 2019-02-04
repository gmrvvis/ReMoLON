#ifndef __REMOLON_REMOTOOSERVERPACKETS_REQUESTFINISHSESSION_H__
#define __REMOLON_REMOTOOSERVERPACKETS_REQUESTFINISHSESSION_H__

#include "ReMoLON_Util/Packet.h"

namespace remolon
{
  namespace remotooserverpackets
  {
    class RequestFinishSession : public remolonUtil::SendablePacket
    {
      public:
        char getOpcode ( );
        void writeImpl ( );
    };
  }
}

#endif