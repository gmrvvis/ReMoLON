#include "frontendserverpackets/RequestNodeInfo.h"

#include <iostream>

namespace remolon
{
  namespace frontendserverpackets
  {
    char RequestNodeInfo::getOpcode ( )
    {
      return 0x00;
    }

    void RequestNodeInfo::readImpl ( )
    {

    }

    void RequestNodeInfo::executePacketAction ( )
    {
      std::cout << "Requested node info from frontend" << std::endl;
    }
  }
}