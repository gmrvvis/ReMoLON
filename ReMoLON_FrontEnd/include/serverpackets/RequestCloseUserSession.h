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
 
#ifndef __REMOLON_FRONTEND_SERVERPACKETS_REQUESTCLOSEUSERSESSION_H__
#define __REMOLON_FRONTEND_SERVERPACKETS_REQUESTCLOSEUSERSESSION_H__

#include <string>

#include "ReMoLON_Util/Packet.h"

namespace remolonFrontend
{
  namespace serverpackets
  {
    class RequestCloseUserSession : public remolonUtil::SendablePacket
    {
      public:
        RequestCloseUserSession ( const std::string & userName_,
                                  const std::string & sessionName_ );
        char getOpcode ( );
        void writeImpl ( );
      private:
        std::string _userName;
        std::string _sessionName;
    };
  }
}

#endif