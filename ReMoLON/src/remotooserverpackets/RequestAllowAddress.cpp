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
      return 0x00;
    }

    void RequestAllowAddress::writeImpl ( )
    {
      writeString ( _address );
    }
  }
}