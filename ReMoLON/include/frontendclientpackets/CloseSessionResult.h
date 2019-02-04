#ifndef __REMOLON_FRONTENDCLIENTPACKETS_CLOSESESSIONRESULT_H__
#define __REMOLON_FRONTENDCLIENTPACKETS_CLOSESESSIONRESULT_H__

#include "ReMoLON_Util/Packet.h"

namespace remolon
{
  namespace frontendclientpackets
  {
    class CloseSessionResult : public remolonUtil::SendablePacket
    {
      public:
        CloseSessionResult ( const std::string & user_,
                             const std::string & session_,
                             int status_ );
        char getOpcode ( );
        void writeImpl ( );
      private:
        std::string _userName;
        std::string _sessioName;
        int _status;
    };
  }
}

#endif