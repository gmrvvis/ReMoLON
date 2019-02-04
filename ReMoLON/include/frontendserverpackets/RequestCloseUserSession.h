#ifndef __REMOLON_FRONTENDSERVERPACKETS_REQUESTCLOSEUSERSESSION_H__
#define __REMOLON_FRONTENDSERVERPACKETS_REQUESTCLOSEUSERSESSION_H__

#include "ReMoLON_Util/Packet.h"

namespace remolon
{
  namespace frontendserverpackets
  {
    class RequestCloseUserSession : public remolonUtil::ReceivablePacket
    {
      public:
        char getOpcode ( );
        void readImpl ( );
        void executePacketAction ( );
      private:
        std::string _userName;
        std::string _sessionName;
    };
  }
}

#endif