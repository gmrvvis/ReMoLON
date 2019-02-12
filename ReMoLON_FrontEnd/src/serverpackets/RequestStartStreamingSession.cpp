#include "serverpackets/RequestStartStreamingSession.h"

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
    }
  }
}