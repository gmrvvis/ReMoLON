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
      closeResult = std::unique_ptr < frontendclientpackets::CloseSessionResult >
                    ( new frontendclientpackets::CloseSessionResult  ( _userName,
                                                                       _sessionName,
                                                                       0 ) );
      clnt->sendPacket ( closeResult );                                                                            
    }
  }
}