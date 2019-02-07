#include "remotooclientpackets/AllowAddressAnswer.h"

namespace remolon
{
  namespace remotooclientpackets
  {
    char AllowAddressAnswer::getOpcode ( )
    {
      return 0x01;
    }

    void AllowAddressAnswer::readImpl ( )
    {
      _targetAddress = readString ( );
      _allowed = readInt ( ) > 0 ? true : false;
    }

    void AllowAddressAnswer::executePacketAction ( )
    {
      
    }
  }
}