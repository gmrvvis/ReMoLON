#include "frontendserverpackets/RequestCloseUserSession.h"

namespace remolon
{
  namespace frontendserverpackets
  {
    char RequestCloseUserSession::getOpcode ( )
    {
      return 0x02;
    }

    void RequestCloseUserSession::readImpl ( )
    {
      _userName = readString ( );
      _sessionName = readString ( );
    }

    void RequestCloseUserSession::executePacketAction ( )
    {

    }
  }
}