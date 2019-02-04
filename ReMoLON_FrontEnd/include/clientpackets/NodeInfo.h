#ifndef __REMOLON_FRONTEND_CLIENTPACKETS_NODEINFO_H__
#define __REMOLON_FRONTEND_CLIENTPACKETS_NODEINFO_H__

#include "ReMoLON_Util/Packet.h"

namespace remolonFrontend
{
  namespace clientpackets
  {
    class NodeInfo : public remolonUtil::ReceivablePacket
    {
      public:
        char getOpcode ( );
        void readImpl ( );
        void executePacketAction ( );
    };
  }
}

#endif