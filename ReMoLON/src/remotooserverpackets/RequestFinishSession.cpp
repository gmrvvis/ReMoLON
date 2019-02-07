#include "remotooserverpackets/RequestFinishSession.h"

namespace remolon
{
  namespace remotooserverpackets
  {
    char RequestFinishSession::getOpcode ( )
    {
      return 0x02;
    }

    void RequestFinishSession::writeImpl ( )
    {
      
    }
  }
}