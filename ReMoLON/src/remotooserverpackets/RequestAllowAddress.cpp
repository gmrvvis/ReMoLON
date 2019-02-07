#include "remotooserverpackets/RequestAllowAddress.h"

namespace remolon
{
  namespace remotooserverpackets
  {
    RequestAllowAddress::RequestAllowAddress ( const std::string & address_ )
     : _address ( address_ )
    {
    }

    char RequestAllowAddress::getOpcode ( )
    {
      return 0x01;
    }

    void RequestAllowAddress::writeImpl ( )
    {
      writeString ( _address );
    }
  }
}