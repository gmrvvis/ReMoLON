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

#include "remotooclientpackets/SessionInfo.h"

#include "SessionManager.h"

namespace remolon
{
  namespace remotooclientpackets
  {
    char SessionInfo::getOpcode ( )
    {
      return 0x00;
    }

    void SessionInfo::readImpl ( )
    {
      _sessionName = readString ( );
      _userOwner = readString ( );
    }

    void SessionInfo::executePacketAction ( )
    {
      StreamingSession *
      session = SessionManager::getInstance ( ).getStreamSession ( _userOwner,
                                                                   _sessionName );
      if ( session )
      {
        session->setConnection ( getConnection ( ) );
      }
    }
  }
}
