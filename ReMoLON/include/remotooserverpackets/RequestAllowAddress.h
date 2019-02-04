#ifndef __REMOLON_REMOTOOSERVERPACKETS_REQUESTALLOWADDRESS_H__
#define __REMOLON_REMOTOOSERVERPACKETS_REQUESTALLOWADDRESS_H__

#include "ReMoLON_Util/Packet.h"

namespace remolon
{
  namespace remotooserverpackets
  {
    class RequestAllowAddress : public remolonUtil::SendablePacket
    {
      public:
        RequestAllowAddress ( const std::string & address_ );
        char getOpcode ( );
        void writeImpl ( );
      private:
        std::string _address;
    };
  }
}

#endif