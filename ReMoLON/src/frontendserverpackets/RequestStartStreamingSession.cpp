#include "frontendserverpackets/RequestStartStreamingSession.h"

#include "SessionManager.h"

#include <iostream>

namespace remolon
{
  namespace frontendserverpackets
  {
    char RequestStartStreamingSession::getOpcode ( )
    {
      return 0x01;
    }

    void RequestStartStreamingSession::readImpl ( )
    {
      _user = readString ( );
      _userAddress = readString ( );
      _sessionName = readString ( );
      _port = readInt ( );
    }

    void RequestStartStreamingSession::executePacketAction ( )
    {
      std::cout << "Received request to start new session for user " << _user << " in port " << _port << " with address "  << _userAddress << std::endl;

      SessionManager::getInstance ( ).startNewSession ( _user,
                                                        _userAddress,
                                                        _sessionName,
                                                        _port );
    }
  }
}