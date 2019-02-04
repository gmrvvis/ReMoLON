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
		remolonUtil::TServerConfig serverConfig;
		serverConfig._bindToAddress = cfg.getProperty ( "serverIPAddressBind" );
		serverConfig._bindToPort = cfg.getIntProperty ( "serverPortBind" );
		serverConfig._keyFilePath = cfg.getProperty ( "keyFilePath" );
		serverConfig._certFilePath = cfg.getProperty ( "certificateFilePath" );
		serverConfig._caFilePath = cfg.getProperty ( "CAFilePath" );

    _server = std::make_unique < remolonUtil::SecureServer > ( serverConfig );
		remolonUtil::SecureServer * srv = _server.get ( );
		srv->registerReceivablePacket < clientpackets::NodeInfo > ( );
		srv->registerReceivablePacket < clientpackets::StartStreamingSessionResult > ( );
		srv->registerReceivablePacket < clientpackets::CloseSessionResult > ( );

		//_server = std::make_unique < remolonUtil::secureServer > ( "127.0.0.1", 7777 );
    _server.get ( )->start ( );
	}

	void SessionManagerServer::shutDown()
	{
    _server.get ( )->shutDown ( );
	}
}
