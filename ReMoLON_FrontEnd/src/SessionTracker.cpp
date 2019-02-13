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

#include "SessionTracker.h"

#include "VisualizationNodeManager.h"
#include "UserManager.h"
#include "serverpackets/RequestStartStreamingSession.h"
#include "serverpackets/RequestCloseUserSession.h"

namespace remolonFrontend
{
  SessionTracker SessionTracker::_INSTANCE;

  SessionTracker & SessionTracker::getInstance ( )
  {
    return _INSTANCE;
  }

  SessionCreationResult SessionTracker::tryCreateSession ( User & user_, 
                                                           const std::string & sessionName_,
                                                           TStreamingSessionInfo * result_ )
  {
    std::unique_lock < std::mutex > lock ( user_.getUserLock ( ) );

    // Check for duplicateNames
    auto it = _openSessions.find ( user_.getUserName ( ) );
    if ( it != _openSessions.end ( ) )
    {
      sessionList & userSessions = it->second;
      auto innerIt = userSessions.find ( sessionName_ );
      if ( innerIt != userSessions.end ( ) )
      {
        return SessionCreationResult::CREATION_FAIL_DUPLICATE_NAME;
      }
    }

    // Try to find a suitable node (max 20 attempts)
    bool slotReserved = false;
    unsigned int choosenPort = 0;

    const int maxAttempts = 20;
    int attempt = 0;

    VisualizationNode * node = nullptr;

    do
    {
      node = VisualizationNodeManager::getInstance ( ).findBestNode ( );

      if ( node == nullptr )
      {
        return SessionCreationResult::CREATION_FAIL_NO_AVAILABLE_NODES;
      }  
      slotReserved = node->tryReserveSlot ( choosenPort );
    } 
    while ( ( node == nullptr  || !slotReserved ) && attempt < maxAttempts );

    // No available node found
    if ( !slotReserved )
    {
      return SessionCreationResult::CREATION_FAIL_NO_AVAILABLE_NODES;
    }

    sessionList & userSessionList = _openSessions [ user_.getUserName ( ) ];
    TStreamingSessionInfo & sessionInfo = userSessionList [ sessionName_ ];
    sessionInfo._sessionName = sessionName_;
    sessionInfo._nodeAddress = node->getAddress ( );
    sessionInfo._nodePublicAddress = node->getPublicAddress ( );
    sessionInfo._nodePort = choosenPort;
    sessionInfo._ownerIp = user_.getAddress ( );
    sessionInfo._ownerUsername = user_.getUserName ( );
    sessionInfo._status = SessionStatus::SESSION_CREATING;

    result_ = &sessionInfo;
    //###
    (void)result_;
    
    remolonUtil::SendablePacketPtr 
    startPacket = std::make_unique < serverpackets::RequestStartStreamingSession > ( user_,
                                                                                     sessionName_,
                                                                                     choosenPort );

    node->sendPacket ( startPacket );

    user_.addSession ( sessionName_, sessionInfo._nodeAddress );

    return SessionCreationResult::CREATION_OK;
  }

  void SessionTracker::createSession ( User & usr_, const std::string & sessionName_, int status_ )
  {
    std::unique_lock < std::mutex > lock ( usr_.getUserLock ( ) );

    TStreamingSessionInfo * info = getSessionInfo ( usr_, sessionName_ );
    if ( info != nullptr )
    {
      info->_status = status_ == 0? SessionStatus::SESSION_RUNNING : SessionStatus::SESSION_CRASHED;
    }
  }

  void SessionTracker::tryFinishSession ( User & user_, 
                                          const std::string & sessionName_ )
  {
    std::unique_lock < std::mutex > lock ( user_.getUserLock ( ) );

    TStreamingSessionInfo * info = getSessionInfo ( user_, sessionName_ );
    if ( info != nullptr )
    {
      info->_status = SessionStatus::SESSION_CLOSING;
      VisualizationNode * node = VisualizationNodeManager::getInstance ( ).getNode ( info->_nodeAddress );

      // We cannot contact the node, delete the session history
      if ( node == nullptr )
      {
        finishSession ( user_, sessionName_ );
      }
      else
      {
        remolonUtil::SendablePacketPtr 
        closeSession = std::make_unique < serverpackets::RequestCloseUserSession > ( user_.getUserName ( ), 
                                                                                          sessionName_ );
        node->sendPacket ( closeSession );
      }
    }
  }

  void SessionTracker::finishSession ( User & user_,
                                       const std::string & sessionName_ )
  {
    auto it = _openSessions.find ( user_.getUserName ( ) );
    if ( it != _openSessions.end ( ) )
    {
      auto innerIt = it->second.find ( sessionName_ );
      if ( innerIt != it->second.end ( ) )
      {
        it->second.erase ( innerIt );
        user_.removeSession ( sessionName_ );
      }
    }
  }

  TStreamingSessionInfo * SessionTracker::getSessionInfo ( User & user_,
                                                          const std::string & sessionName_ )
  {
    auto it = _openSessions.find ( user_.getUserName ( ) );
    if ( it != _openSessions.end ( ) )
    {
      auto innerIt = it->second.find ( sessionName_ );
      if ( innerIt != it->second.end ( ) )
      {
        return &(innerIt->second);
      }
    }

    return nullptr;
  }

  const std::unordered_map < std::string, TStreamingSessionInfo > & SessionTracker::getUserSessions ( User & user_ )
  {
    sessionList & list = _openSessions [ user_.getUserName ( ) ];
    
    return list;
  }
}
