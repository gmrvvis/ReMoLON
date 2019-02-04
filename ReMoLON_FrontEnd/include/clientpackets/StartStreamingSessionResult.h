#ifndef __REMOLON_FRONTEND_CLIENTPACKETS_STARTSTREAMINGSESSIONRESULT_H__
#define __REMOLON_FRONTEND_CLIENTPACKETS_STARTSTREAMINGSESSIONRESULT_H__

#include "ReMoLON_Util/Packet.h"

namespace remolonFrontend
{
  namespace clientpackets
  {
    class StartStreamingSessionResult : public remolonUtil::ReceivablePacket
    {
      public:
        char getOpcode ( );
        void readImpl ( );
        void executePacketAction ( );
      private:
        std::string _userName;
        std::string _sessionName;
        int _creationStatus;
    };
  }
}

#endif