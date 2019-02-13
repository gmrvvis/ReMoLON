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
 
#include "clientpackets/StartStreamingSessionResult.h"

#include "UserManager.h"
#include "SessionTracker.h"

namespace remolonFrontend
{
  namespace clientpackets
  {
    char StartStreamingSessionResult::getOpcode ( )
    {
      return 0x01;
    }

    void StartStreamingSessionResult::readImpl ( )
    {
      _userName = readString ( );
      _sessionName = readString ( );
      _creationStatus = readInt ( );
    }

    void StartStreamingSessionResult::executePacketAction ( )
    {
      User * usr = UserManager::getInstance ( ).getUserByName ( _userName );
      if ( usr == nullptr )
      {
        return;
      }

      SessionTracker::getInstance ( ).createSession ( *usr, _sessionName, _creationStatus );
    }
  }
}