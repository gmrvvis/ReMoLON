/*
 * Copyright (c) 2019 CCS/UPM - GMRV/URJC.
 *
 * Authors: Nadir Román Guerrero <nadir.roman@urjc.es>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
 
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

  void SessionManager::setPublicAddress ( const std::string & address_ )
  {
    _publicAddress = address_;
  }

  void SessionManager::addAvailablePort ( uint16_t port_ )
  {
    _usedPorts [ port_ ] = false;
  }

  void SessionManager::addAvailableSocketPort ( uint16_t socketPort_ )
  {
    _usedSockPorts [ socketPort_ ] = false;
  }

  void SessionManager::addAvailableRTCPort ( uint16_t rtcPort_ )
  {
    _usedRTCPorts [ rtcPort_ ] = false;
  }

  uint16_t SessionManager::findAvailablePort ( std::unordered_map < uint16_t, bool> & source_ )
  {
    auto it = source_.begin ( );
    for ( ; it != source_.end ( ); it++ )
    {
      if ( !(it->second) )
      {
        it->second = true;
        return it->first;
      }
    }

    return 0;
  }

  void SessionManager::startNewSession ( const std::string & ownerName_,
                                         const std::string & ownerAddress_,
                                         const std::string & sessionName_,
                                         uint16_t port_ )
  {
    std::unique_lock < std::mutex > lock ( _mtx );

    remolonUtil::Client * 
    client = Node::getInstance ( ).getFrontendClient ( );

    remolonUtil::SendablePacketPtr result;

    auto portIt = _usedPorts.find ( port_ );

    uint16_t sockPort = findAvailablePort ( _usedSockPorts );
    uint16_t rtcPort = findAvailablePort ( _usedRTCPorts );

    // Requested not existing port (shouldn't happen) OR port is in use
    if ( portIt == _usedPorts.end ( ) || portIt->second || sockPort == 0 || rtcPort == 0 )
    {
      result = std::unique_ptr < frontendclientpackets::StartStreamingSessionResult >
               ( new frontendclientpackets::StartStreamingSessionResult ( ownerName_,
                                                                          sessionName_,
                                                                          SESSION_LAUNCH_PORT_UNAVAILABLE ) );
      client->sendPacket ( result );
      std::cout << "Couldnt start session: port in use" << std::endl;                                                                                
      return;
    }

    portIt->second = true;

    sessionList & sList = _streamSessions [ ownerName_ ];

    // User has already a session with same name on this node
    if ( sList.find ( sessionName_ ) != sList.end ( ) )
    {
      result = std::unique_ptr < frontendclientpackets::StartStreamingSessionResult >
               ( new frontendclientpackets::StartStreamingSessionResult ( ownerName_,
                                                                          sessionName_,
                                                                          SESSION_LAUNCH_DUPLICATE_NAME ) );
      client->sendPacket ( result );                                                                                  
      return;
    }

    StreamingSession & session = sList [ sessionName_ ];
    session.setSessionName ( sessionName_ );
    session.setSessionOwner ( ownerName_ );
    session.setSessionOwnerAddress ( ownerAddress_ );
    session.setSessionPorts ( port_, sockPort, rtcPort );

    if ( session.tryLaunchSession ( ) )
    {
      result = std::unique_ptr < frontendclientpackets::StartStreamingSessionResult >
               ( new frontendclientpackets::StartStreamingSessionResult ( ownerName_,
                                                                          sessionName_,
                                                                          SESSION_LAUNCH_OK ) );
    }
    else
    {
      result = std::unique_ptr < frontendclientpackets::StartStreamingSessionResult > 
               ( new frontendclientpackets::StartStreamingSessionResult ( ownerName_,
                                                                          sessionName_,
                                                                          SESSION_LAUNCH_CRASHED ) );
    }
    
    client->sendPacket ( result );
  }

  void SessionManager::finishSession ( const std::string & ownerName_,
                                       const std::string & sessionName_ )
  {
    std::unique_lock < std::mutex > lock ( _mtx );
    
    sessionList & sList = _streamSessions [ ownerName_ ];
    auto it = sList.find ( sessionName_ );

    if ( it == sList.end ( ) )
    {
      return;
    }

    StreamingSession & session = it->second;

    // Send signal to process
    session.finishSession ( );
  }

  void SessionManager::clearSession ( StreamingSession & session )
  {
    std::unique_lock < std::mutex > lock ( _mtx );

    // Release used ports
    _usedPorts [ session.getSessionPort ( ) ] = false;
    _usedSockPorts [ session.getSessionSocketPort ( ) ] = false;
    _usedRTCPorts [ session.getSessionRTCPort ( ) ] = false;

    sessionList & sList = _streamSessions [ session.getSessionOwner ( ) ];
    auto it = sList.find ( session.getSessionName ( ) );

    if ( it == sList.end ( ) )
    {
      return;
    }

    sList.erase ( it );
  }

  void SessionManager::finishAllSessions ( )
  {
    std::unique_lock < std::mutex > lock ( _mtx );
    auto it = _streamSessions.begin ( );
    for ( ; it != _streamSessions.end ( ); it++ )
    {
      auto innerIt = it->second.begin ( );
      for ( ; innerIt != it->second.end ( ); innerIt++ )
      {
        innerIt->second.finishSession ( );
      }
    }
  }

  StreamingSession * SessionManager::getStreamSession ( const std::string & ownerName_,
                                                        const std::string & sessionName_ )
  {
    sessionList & sList = _streamSessions [ ownerName_ ];
    auto it = sList.find ( sessionName_ );
    if ( it != sList.end ( ) )
    {
      return &(it->second);
    }

    return nullptr;
  }

  const std::string & SessionManager::getPublicAddress ( )
  {
    return _publicAddress;
  }

  const std::unordered_map < uint16_t, bool > & SessionManager::getPortList ( )
  {
    return _usedPorts;
  }
}