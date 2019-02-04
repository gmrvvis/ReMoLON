#include "Node.h"

#include <iostream>

#include "ReMoLON_Util/Config.h"

#include "SessionManager.h"
#include "frontendserverpackets/RequestCloseUserSession.h"
#include "frontendserverpackets/RequestNodeInfo.h"
#include "frontendserverpackets/RequestStartStreamingSession.h"

namespace remolon
{
  Node Node::_INSTANCE;

  Node & Node::getInstance ( )
  {
    return _INSTANCE;
  }

  void Node::initialize ( const std::string & clientConfigFilePath_ )
  {
    remolonUtil::Config clientCfg ( clientConfigFilePath_ );
    remolonUtil::TClientConfig clientConfig;

    clientConfig._serverAddress = clientCfg.getProperty ( "serverAddress" );
		clientConfig._serverPort = clientCfg.getIntProperty ( "serverPort" );
		clientConfig._keyFilePath = clientCfg.getProperty ( "keyFilePath" );
		clientConfig._certFilePath = clientCfg.getProperty ( "certificateFilePath" );
		clientConfig._caFilePath = clientCfg.getProperty ( "CAFilePath" );

    // Initialize client
    _client = std::make_unique < remolonUtil::SecureClient > ( clientConfig );
    remolonUtil::SecureClient * cPtr = _client.get ( );
    cPtr->registerReceivablePacket < frontendserverpackets::RequestNodeInfo > ( );
    cPtr->registerReceivablePacket < frontendserverpackets::RequestStartStreamingSession > ( );
    cPtr->registerReceivablePacket < frontendserverpackets::RequestCloseUserSession > ( );
    cPtr->connect ( );

    // Initialize server
    std::vector < std::string > availablePorts = clientCfg.getPropertyList ( "availablePorts", "," );
    for ( auto & str : availablePorts )
    {
      try
      {
        uint16_t newPort = static_cast < uint16_t > ( std::stoi ( str ) );
        SessionManager::getInstance ( ).addAvailablePort ( newPort );
      }
      catch ( const std::exception& e )
      {
        std::cerr << "ReMoLON: Error parsing the list of available ports: " << e.what() << std::endl;
      }
      
    }
  }

  remolonUtil::SecureClient & Node::getFrontendClient ( )
  {
    return *( _client.get ( ) );
  }

  remolonUtil::SecureServer & Node::getNodeServer ( )
  {
    return *( _server.get ( ) );
  }

  void Node::sendNodeInfo ( )
  {

  }
}