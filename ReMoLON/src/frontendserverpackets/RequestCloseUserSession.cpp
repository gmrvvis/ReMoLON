#include "frontendserverpackets/RequestCloseUserSession.h"

#include "SessionManager.h"
#include "Node.h"
#include "frontendclientpackets/CloseSessionResult.h"

namespace remolon
{
  namespace frontendserverpackets
  {
    char RequestCloseUserSession::getOpcode ( )
    {
      return 0x02;
    }

    void RequestCloseUserSession::readImpl ( )
    {
      _userName = readString ( );
      _sessionName = readString ( );
    }

    void RequestCloseUserSession::executePacketAction ( )
    {
      SessionManager::getInstance ( ).finishSession ( _userName, _sessionName );

      remolonUtil::Client * clnt = Node::getInstance ( ).getFrontendClient ( );

      remolonUtil::SendablePacketPtr 
      closeResult = std::make_unique < frontendclientpackets::CloseSessionResult > ( _userName,
                                                                                     _sessionName,
                                                                                     0 );
      clnt->sendPacket ( closeResult );                                                                            
    }
  }
}