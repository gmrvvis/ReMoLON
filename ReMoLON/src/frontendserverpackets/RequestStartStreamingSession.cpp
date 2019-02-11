#include "frontendserverpackets/RequestStartStreamingSession.h"

#include "SessionManager.h"

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
      SessionManager::getInstance ( ).startNewSession ( _user,
                                                        _userAddress,
                                                        _sessionName,
                                                        _port );
    }
  }
}