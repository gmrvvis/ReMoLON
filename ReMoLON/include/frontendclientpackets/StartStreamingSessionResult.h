#ifndef __REMOLON_FRONTENDCLIENTPACKETS_STARTSTREAMINGSESSIONRESULT_H__
#define __REMOLON_FRONTENDCLIENTPACKETS_STARTSTREAMINGSESSIONRESULT_H__

#include "ReMoLON_Util/Packet.h"

namespace remolon
{
  namespace frontendclientpackets
  {
    class StartStreamingSessionResult : public remolonUtil::SendablePacket
    {
      public:
        StartStreamingSessionResult ( const std::string & user_,
                                      const std::string & session_,
                                      int status_ );
        char getOpcode ( );
        void writeImpl ( );
      private:
        std::string _userName;
        std::string _sessionName;
        int _status;
    };
  }
}

#endif