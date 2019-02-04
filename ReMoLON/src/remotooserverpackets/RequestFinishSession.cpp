#include "remotooserverpackets/RequestFinishSession.h"

namespace remolon
{
  namespace remotooserverpackets
  {
    char RequestFinishSession::getOpcode ( )
    {
      return 0x01;
    }

    void RequestFinishSession::writeImpl ( )
    {
      
    }
  }
}