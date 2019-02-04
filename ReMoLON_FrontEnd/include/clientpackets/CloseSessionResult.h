#ifndef __REMOLON_FRONTEND_CLIENTPACKETS_CLOSESESSIONRESULT_H__
#define __REMOLON_FRONTEND_CLIENTPACKETS_CLOSESESSIONRESULT_H__

#include "ReMoLON_Util/Packet.h"

namespace remolonFrontend
{
  namespace clientpackets
  {
    class CloseSessionResult : public remolonUtil::ReceivablePacket
    {
      public: 
        char getOpcode ( );
        void readImpl ( );
        void executePacketAction ( );

      private:
        std::string _user;
        std::string _sessionName;
        int _closeStatus;
    };
  }
}

#endif