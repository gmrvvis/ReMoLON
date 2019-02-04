#include "clientpackets/CloseSessionResult.h"

#include "SessionTracker.h"
#include "UserManager.h"

namespace remolonFrontend
{
  namespace clientpackets
  {
    char CloseSessionResult::getOpcode ( )
    {
      return 0x02;
    }

    void CloseSessionResult::readImpl ( )
    {
      _user = readString ( );
      _sessionName = readString ( );
      _closeStatus = readInt ( );
    }

    void CloseSessionResult::executePacketAction ( )
    {
      User * usr = UserManager::getInstance ( ).getUserByName ( _user );
      if ( usr != nullptr )
      {
        SessionTracker::getInstance ( ).finishSession ( *usr, _sessionName );
      }
    }
  }
}