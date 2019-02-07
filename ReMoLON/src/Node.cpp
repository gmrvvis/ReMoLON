#include "Node.h"

#include <iostream>

#include "ReMoLON_Util/Config.h"

#include "SessionManager.h"

#include "frontendserverpackets/RequestCloseUserSession.h"
#include "frontendserverpackets/RequestNodeInfo.h"
#include "frontendserverpackets/RequestStartStreamingSession.h"

#include "remotooclientpackets/SessionInfo.h"
#include "remotooclientpackets/AllowAddressAnswer.h"

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

    _remotooBinDir = clientCfg.getProperty ( "remotooBinaryDir" );
    _remolonXinitrcDir = clientCfg.getProperty ( "remolonXinitrcDir" );

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
    remolonUtil::TServerConfig serverConfig;
    serverConfig._bindToAddress = clientCfg.getProperty ( "innerServerAddress" );
    serverConfig._bindToPort = clientCfg.getIntProperty ( "innerServerPort" );
    
    _server = std::make_unique < remolonUtil::Server > ( serverConfig );
    remolonUtil::Server * sPtr = _server.get ( );
    sPtr->registerReceivablePacket < remotooclientpackets::SessionInfo > ( );
    sPtr->registerReceivablePacket < remotooclientpackets::AllowAddressAnswer > ( );

    // Initialize streaming configuration
    std::string publicAddr = clientCfg.getProperty ( "publicAddress" );
    SessionManager::getInstance ( ).setPublicAddress ( publicAddr );

    std::vector < std::string > availablePorts = clientCfg.getPropertyList ( "availableWebPorts", "," );
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

    std::vector < std::string > availableSockPorts = clientCfg.getPropertyList ( "availableWebSockPorts", "," );
    for ( auto & str : availableSockPorts )
    {
      try
      {
        uint16_t newPort = static_cast < uint16_t > ( std::stoi ( str ) );
        SessionManager::getInstance ( ).addAvailableSocketPort ( newPort );
      }
      catch ( const std::exception& e )
      {
        std::cerr << "ReMoLON: Error parsing the list of available SOCKET ports: " << e.what() << std::endl;
      }
    }

    std::vector < std::string > availableRtcPorts = clientCfg.getPropertyList ( "availableWebRtcPorts", "," );
    for ( auto & str : availableRtcPorts )
    {
      try
      {
        uint16_t newPort = static_cast < uint16_t > ( std::stoi ( str ) );
        SessionManager::getInstance ( ).addAvailableRTCPort ( newPort );
      }
      catch ( const std::exception& e )
      {
        std::cerr << "ReMoLON: Error parsing the list of available RTC ports: " << e.what() << std::endl;
      }
    }
  }
  
  const std::string & Node::getRemotooBinaryDir ( )
  {
    return _remotooBinDir;
  }

  const std::string & Node::getRemolonXinitrcBinaryDir ( )
  {
    return _remolonXinitrcDir;
  }

  remolonUtil::SecureClient & Node::getFrontendClient ( )
  {
    return *( _client.get ( ) );
  }

  remolonUtil::Server & Node::getNodeServer ( )
  {
    return *( _server.get ( ) );
  }
}