#ifndef __REMOLON_FRONTENDSERVERPACKETS_REQUESTNODEINFO_H__
#define __REMOLON_FRONTENDSERVERPACKETS_REQUESTNODEINFO_H__

#include "ReMoLON_Util/Packet.h"

namespace remolon
{
  namespace frontendserverpackets
  {
    class RequestNodeInfo : public remolonUtil::ReceivablePacket
    {
      public:
        char getOpcode ( );
        void readImpl ( );
        void executePacketAction ( );
    };
  }
}

#endif