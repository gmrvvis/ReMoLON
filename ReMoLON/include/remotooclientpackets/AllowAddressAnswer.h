#ifndef __REMOLON_REMOTOOCLIENTPACKETS_ALLOWADDRESSANSWER_H__
#define __REMOLON_REMOTOOCLIENTPACKETS_ALLOWADDRESSANSWER_H__

#include "ReMoLON_Util/Packet.h"

namespace remolon
{
  namespace remotooclientpackets
  {
    class AllowAddressAnswer : public remolonUtil::ReceivablePacket
    {
      public:
        char getOpcode ( );
        void readImpl ( );
        void executePacketAction ( );
      private:
        std::string _targetAddress;
        bool _allowed;
    };
  }
}

#endif