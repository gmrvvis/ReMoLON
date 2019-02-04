#include "frontendclientpackets/CloseSessionResult.h"

namespace remolon
{
  namespace frontendclientpackets
  {
    CloseSessionResult::CloseSessionResult ( const std::string & user_,
                                             const std::string & session_,
                                             int status_ )
     : _userName ( user_ )
     , _sessioName ( session_ )
     , _status ( status_ )
    {
    }

    char CloseSessionResult::getOpcode ( )
    {
      return 0x02;
    }

    void CloseSessionResult::writeImpl ( )
    {
      writeString ( _userName );
      writeString ( _sessioName );
      writeInt ( _status );
    }    
  }
}