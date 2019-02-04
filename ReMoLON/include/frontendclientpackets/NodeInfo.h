#ifndef __REMOLON_FRONTENDCLIENTPACKETS_NODEINFO_H__
#define __REMOLON_FRONTENDCLIENTPACKETS_NODEINFO_H__

#include  "ReMoLON_Util/Packet.h"

namespace remolon
{
  namespace frontendclientpackets
  {
    class NodeInfo : public remolonUtil::SendablePacket
    {
      public:
        char getOpcode ( );
        void writeImpl ( );
    };
  }
}

#endif