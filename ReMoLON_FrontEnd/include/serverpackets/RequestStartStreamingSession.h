#ifndef __REMOLON_FRONTEND_SERVERPACKETS_REQUESTSTARTSTREAMINGSESSION_H__
#define __REMOLON_FRONTEND_SERVERPACKETS_REQUESTSTARTSTREAMINGSESSION_H__

#include "ReMoLON_Util/Packet.h"

#include "User.h"

namespace remolonFrontend
{
  namespace serverpackets
  {
    class RequestStartStreamingSession : public remolonUtil::SendablePacket
    {
      public:
        RequestStartStreamingSession ( User & user_,
                                       const std::string & sessionName_,
                                       unsigned int choosenPort_  );
        char getOpcode ( );
        void writeImpl ( );

      private:
        std::string _userAddress;
        std::string _userName;
        std::string _sessionName;
        unsigned int _port;
    };
  }
}

#endif