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
		_server = std::make_unique < remolonUtil::RawServer > ( bindToAddress, bindToPort );
#else
		std::string keyFilePath = cfg.getProperty ( "keyFilePath" );
		std::string certFilePath = cfg.getProperty ( "certificateFilePath" );
		std::string caFilePath = cfg.getProperty ( "CAFilePath" );

    _server = std::make_unique < remolonUtil::SecureServer > ( bindToAddress, bindToPort,
																															 keyFilePath,
																															 certFilePath,
																															 caFilePath );
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
