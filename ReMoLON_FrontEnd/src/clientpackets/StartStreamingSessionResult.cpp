#include "clientpackets/StartStreamingSessionResult.h"

#include <iostream>

#include "UserManager.h"
#include "SessionTracker.h"

namespace remolonFrontend
{
  namespace clientpackets
  {
    char StartStreamingSessionResult::getOpcode ( )
    {
      return 0x01;
    }

    void StartStreamingSessionResult::readImpl ( )
    {
      _userName = readString ( );
      _sessionName = readString ( );
      _creationStatus = readInt ( );
    }

    void StartStreamingSessionResult::executePacketAction ( )
    {
      User * usr = UserManager::getInstance ( ).getUserByName ( _userName );
      if ( usr == nullptr )
      {
        return;
      }

      std::cout << "Received answer for " << _userName << " and session " << _sessionName << " status " << _creationStatus << std::endl;

      SessionTracker::getInstance ( ).createSession ( *usr, _sessionName, _creationStatus );
    }
  }
}