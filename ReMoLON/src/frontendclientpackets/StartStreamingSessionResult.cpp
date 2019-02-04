#include "frontendclientpackets/StartStreamingSessionResult.h"

namespace remolon
{
  namespace frontendclientpackets
  {
    StartStreamingSessionResult::StartStreamingSessionResult ( const std::string & user_,
                                                               const std::string & session_,
                                                               int status_ )
     : _userName ( user_ )
     , _sessionName ( session_ )
     , _status ( status_ )
    {
    }

    char StartStreamingSessionResult::getOpcode ( )
    {
      return 0x01;
    }

    void StartStreamingSessionResult::writeImpl ( )
    {
      writeString ( _userName );
      writeString ( _sessionName );
      writeInt ( _status );
    }
  }
}