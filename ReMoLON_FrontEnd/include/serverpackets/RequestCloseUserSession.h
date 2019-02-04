#ifndef __REMOLON_FRONTEND_SERVERPACKETS_REQUESTCLOSEUSERSESSION_H__
#define __REMOLON_FRONTEND_SERVERPACKETS_REQUESTCLOSEUSERSESSION_H__

#include <string>

#include "ReMoLON_Util/Packet.h"

namespace remolonFrontend
{
  namespace serverpackets
  {
    class RequestCloseUserSession : public remolonUtil::SendablePacket
    {
      public:
        RequestCloseUserSession ( const std::string & userName_,
                                  const std::string & sessionName_ );
        char getOpcode ( );
        void writeImpl ( );
      private:
        std::string _userName;
        std::string _sessionName;
    };
  }
}

#endif