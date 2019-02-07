#ifndef __REMOLON_REMOTOOCLIENTPACKETS_SESSIONINFO_H__
#define __REMOLON_REMOTOOCLIENTPACKETS_SESSIONINFO_H__

#include "ReMoLON_Util/Packet.h"

namespace remolon
{
  namespace remotooclientpackets
  {
    class SessionInfo : public remolonUtil::ReceivablePacket
    {
      public:
        char getOpcode ( );
        void readImpl ( );
        void executePacketAction ( );
      private:
        std::string _sessionName;
        std::string _userOwner;
    };
  }
}

#endif