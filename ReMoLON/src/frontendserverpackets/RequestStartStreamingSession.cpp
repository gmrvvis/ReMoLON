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
 
#include "frontendserverpackets/RequestStartStreamingSession.h"

#include "SessionManager.h"

namespace remolon
{
  namespace frontendserverpackets
  {
    char RequestStartStreamingSession::getOpcode ( )
    {
      return 0x01;
    }

    void RequestStartStreamingSession::readImpl ( )
    {
      _user = readString ( );
      _userAddress = readString ( );
      _sessionName = readString ( );
      _port = readInt ( );
    }

    void RequestStartStreamingSession::executePacketAction ( )
    {
      SessionManager::getInstance ( ).startNewSession ( _user,
                                                        _userAddress,
                                                        _sessionName,
                                                        _port );
    }
  }
}