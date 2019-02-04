#include "serverpackets/RequestStartStreamingSession.h"

#include <iostream>

namespace remolonFrontend
{
  namespace serverpackets
  {
    RequestStartStreamingSession::RequestStartStreamingSession ( User & user_,
                                                                 const std::string & sessionName_,
                                                                 unsigned int port_ )
    {
      _userName = user_.getUserName ( );
      _userAddress = user_.getAddress ( );
      _sessionName = sessionName_;
      _port = port_;
    }

    char RequestStartStreamingSession::getOpcode ( )
    {
      return 0x01;
    }

    void RequestStartStreamingSession::writeImpl ( )
    {
      writeString ( _userName );
      writeString ( _userAddress );
      writeString ( _sessionName );
      writeInt ( _port );

      std::cout << "Send request start new session with user " << _userName << " in port " << _port << std::endl;
    }
  }
}