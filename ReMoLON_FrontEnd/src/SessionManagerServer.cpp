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
 
#include "SessionManagerServer.h"

#include "ReMoLON_Util/Config.h"

#include "clientpackets/CloseSessionResult.h"
#include "clientpackets/NodeInfo.h"
#include "clientpackets/StartStreamingSessionResult.h"

namespace remolonFrontend
{
  SessionManagerServer SessionManagerServer::_INSTANCE;

  SessionManagerServer & SessionManagerServer::getInstance ( )
  {
    return _INSTANCE;
  }

	SessionManagerServer::SessionManagerServer()
	{

	}

	SessionManagerServer::~SessionManagerServer()
	{

	}

	void SessionManagerServer::start()
	{
    remolonUtil::Config cfg ( "./frontendServerConfig.cfg" );
		
		std::string bindToAddress = cfg.getProperty ( "serverIPAddressBind" );
		std::uint16_t bindToPort = cfg.getIntProperty ( "serverPortBind" );

#ifdef REMOLON_NO_SSL_
		_server = std::unique_ptr < remolonUtil::RawServer > ( new remolonUtil::RawServer ( bindToAddress, 
                                                                                        bindToPort ) );
#else
		std::string keyFilePath = cfg.getProperty ( "keyFilePath" );
		std::string certFilePath = cfg.getProperty ( "certificateFilePath" );
		std::string caFilePath = cfg.getProperty ( "CAFilePath" );

    server = std::unique_ptr < remolonUtil::SecureServer > ( new remolonUtil::SecureServer ( bindToAddress, 
                                                                                             bindToPort,
																															                               keyFilePath,
																															                               certFilePath,
																															                               caFilePath ) );
#endif
		remolonUtil::AbstractServer * srv = _server.get ( );
		srv->registerReceivablePacket < clientpackets::NodeInfo > ( );
		srv->registerReceivablePacket < clientpackets::StartStreamingSessionResult > ( );
		srv->registerReceivablePacket < clientpackets::CloseSessionResult > ( );

		srv->start ( );
	}

	void SessionManagerServer::shutDown()
	{
    _server.get ( )->shutDown ( );
	}
}
