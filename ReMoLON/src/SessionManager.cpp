#include "SessionManager.h"

#include <iostream>

#include "Node.h"
#include "frontendclientpackets/StartStreamingSessionResult.h"

namespace remolon
{
  SessionManager SessionManager::_INSTANCE;

  SessionManager & SessionManager::getInstance ( )
  {
    return _INSTANCE;
  }

  void SessionManager::addAvailablePort ( uint16_t port_ )
  {
    _usedPorts [ port_ ] = false;
  }

  void SessionManager::startNewSession ( const std::string & ownerName_,
                                         const std::string & ownerAddress_,
                                         const std::string & sessionName_,
                                         uint16_t port_ )
  {
    std::unique_lock < std::mutex > lock ( _mtx );

    remolonUtil::SecureClient & 
    client = Node::getInstance ( ).getFrontendClient ( );

    remolonUtil::SendablePacketPtr result;

    auto portIt = _usedPorts.find ( port_ );

    // Requested not existing port (shouldn't happen) OR port is in use
    if ( portIt == _usedPorts.end ( ) || portIt->second )
    {
      result = std::make_unique < frontendclientpackets::StartStreamingSessionResult > ( ownerName_,
                                                                                         sessionName_,
                                                                                         SESSION_LAUNCH_PORT_UNAVAILABLE );
      client.sendPacket ( result );
      std::cout << "Couldnt start session: port in use" << std::endl;                                                                                
      return;
    }

    portIt->second = true;

    sessionList & sList = _streamSessions [ ownerName_ ];

    // User has already a session with same name on this node
    if ( sList.find ( sessionName_ ) != sList.end ( ) )
    {
      result = std::make_unique < frontendclientpackets::StartStreamingSessionResult > ( ownerName_,
                                                                                         sessionName_,
                                                                                         SESSION_LAUNCH_DUPLICATE_NAME );
      client.sendPacket ( result );                                                                                  
      return;
    }

    StreamingSession & session = sList [ sessionName_ ];
    session.setSessionName ( sessionName_ );
    session.setSessionOwner ( ownerName_ );
    session.setSessionOwnerAddress ( ownerAddress_ );
    session.setSessionPort ( port_ );

    if ( session.tryLaunchSession ( ) )
    {
      result = std::make_unique < frontendclientpackets::StartStreamingSessionResult > ( ownerName_,
                                                                                         sessionName_,
                                                                                         SESSION_LAUNCH_OK );
    }
    else
    {
      result = std::make_unique < frontendclientpackets::StartStreamingSessionResult > ( ownerName_,
                                                                                         sessionName_,
                                                                                         SESSION_LAUNCH_CRASHED );
    }
    
    std::cout << "Sending answer to frontend" << std::endl;
    client.sendPacket ( result );
  }

  const std::unordered_map < uint16_t, bool > & SessionManager::getPortList ( )
  {
    return _usedPorts;
  }
}