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

    _remotooBinDir = clientCfg.getProperty ( "remotooBinaryDir" );
    _remolonXinitrcDir = clientCfg.getProperty ( "remolonXinitrcDir" );

    std::string frontendServerAddress = clientCfg.getProperty ( "frontendServerAddress" );
		std::uint16_t frontendServerPort = clientCfg.getIntProperty ( "frontendServerPort" );

#ifdef REMOLON_NO_SSL_
    _client = std::make_unique < remolonUtil::RawClient > ( frontendServerAddress,
                                                            frontendServerPort );
#else
		std::string keyFilePath = clientCfg.getProperty ( "keyFilePath" );
		std::string certFilePath = clientCfg.getProperty ( "certificateFilePath" );
		std::string caFilePath = clientCfg.getProperty ( "CAFilePath" );

    // Initialize client
    _client = std::make_unique < remolonUtil::SecureClient > ( frontendServerAddress,
                                                               frontendServerPort,
                                                               keyFilePath,
                                                               certFilePath,
                                                               caFilePath );
#endif
    remolonUtil::Client * cPtr = _client.get ( );
    cPtr->registerReceivablePacket < frontendserverpackets::RequestNodeInfo > ( );
    cPtr->registerReceivablePacket < frontendserverpackets::RequestStartStreamingSession > ( );
    cPtr->registerReceivablePacket < frontendserverpackets::RequestCloseUserSession > ( );
    cPtr->connect ( );

    // Initialize server
    std::string bindToAddress = clientCfg.getProperty ( "innerServerAddress" );
    std::uint16_t bindToPort = clientCfg.getIntProperty ( "innerServerPort" );
    
    _server = std::make_unique < remolonUtil::RawServer > ( bindToAddress, bindToPort );
    remolonUtil::RawServer * sPtr = _server.get ( );
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

  remolonUtil::Client * Node::getFrontendClient ( )
  {
    return _client.get ( );
  }

  remolonUtil::AbstractServer * Node::getNodeServer ( )
  {
    return _server.get ( );
  }
}