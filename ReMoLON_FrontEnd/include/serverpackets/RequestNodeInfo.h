#ifndef __REMOLON_FRONTEND_SERVERPACKETS_REQUESTNODEINFO_H__
#define __REMOLON_FRONTEND_SERVERPACKETS_REQUESTNODEINFO_H__

#include "ReMoLON_Util/Packet.h"

namespace remolonFrontend
{
  namespace serverpackets
  {
    class RequestNodeInfo : public remolonUtil::SendablePacket
    {
      public:
        char getOpcode ( );
        void writeImpl ( );
    };
  }
}

#endif