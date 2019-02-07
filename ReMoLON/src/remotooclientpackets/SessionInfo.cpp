#include "remotooclientpackets/SessionInfo.h"

#include "SessionManager.h"

#include <iostream>

namespace remolon
{
  namespace remotooclientpackets
  {
    char SessionInfo::getOpcode ( )
    {
      return 0x00;
    }

    void SessionInfo::readImpl ( )
    {
      _sessionName = readString ( );
      _userOwner = readString ( );
    }

    void SessionInfo::executePacketAction ( )
    {
      StreamingSession *
      session = SessionManager::getInstance ( ).getStreamSession ( _userOwner,
                                                                   _sessionName );
      std::cout << "New session connected" << std::endl;

      if ( session )
      {
        session->setConnection ( getConnection ( ) );
      }
    }
  }
}
