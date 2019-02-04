#include "serverpackets/RequestCloseUserSession.h"

namespace remolonFrontend
{
  namespace serverpackets
  {
    RequestCloseUserSession::RequestCloseUserSession ( const std::string & userName_,
                                                       const std::string & sessionName_ )
     : _userName ( userName_ )
     , _sessionName ( sessionName_ )
    {
    }
     
    char RequestCloseUserSession::getOpcode ( )
    {
      return 0x02;
    }
      
    void RequestCloseUserSession::writeImpl ( )
    {
      writeString ( _userName );
      writeString ( _sessionName );
    }
  }
}