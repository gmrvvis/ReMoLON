#include "serverpackets/RequestNodeInfo.h"

namespace remolonFrontend
{
  namespace serverpackets
  {
    char RequestNodeInfo::getOpcode ( )
    {
      return 0x00;
    }

    void RequestNodeInfo::writeImpl ( )
    {
      
    }
  }
}