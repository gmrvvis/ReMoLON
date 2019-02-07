#include "remotooserverpackets/RequestSessionInfo.h"

namespace remolon
{
  namespace remotooserverpackets
  {
    char RequestSessionInfo::getOpcode ( )
    {
      return 0x00;
    }

    void RequestSessionInfo::writeImpl ( )
    {
      
    }
  }
}