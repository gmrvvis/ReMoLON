#ifndef __REMOLON_FRONTENDSERVERPACKETS_REQUESTSTARTSTREAMINGSESSION_H__
#define __REMOLON_FRONTENDSERVERPACKETS_REQUESTSTARTSTREAMINGSESSION_H__

#include "ReMoLON_Util/Packet.h"

namespace remolon
{
  namespace frontendserverpackets
  {
    class RequestStartStreamingSession : public remolonUtil::ReceivablePacket
    {
      public:
        char getOpcode ( );
        void readImpl ( );
        void executePacketAction ( );
      private:
        std::string _user;
        std::string _userAddress;
        std::string _sessionName;
        int _port;
    };  
  }
}

#endif