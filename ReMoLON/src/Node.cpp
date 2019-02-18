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
    _client = std::unique_ptr < remolonUtil::RawClient >
              ( new remolonUtil::RawClient ( frontendServerAddress,
                                             frontendServerPort ) );
#else
		std::string keyFilePath = clientCfg.getProperty ( "keyFilePath" );
		std::string certFilePath = clientCfg.getProperty ( "certificateFilePath" );
		std::string caFilePath = clientCfg.getProperty ( "CAFilePath" );

    // Initialize client
    _client = std::unique_ptr < remolonUtil::SecureClient > 
              ( new remolonUtil::SecureClient ( frontendServerAddress,
                                                frontendServerPort,
                                                keyFilePath,
                                                certFilePath,
                                                caFilePath ) );
#endif
    remolonUtil::Client * cPtr = _client.get ( );
    cPtr->registerReceivablePacket < frontendserverpackets::RequestNodeInfo > ( );
    cPtr->registerReceivablePacket < frontendserverpackets::RequestStartStreamingSession > ( );
    cPtr->registerReceivablePacket < frontendserverpackets::RequestCloseUserSession > ( );
    cPtr->connect ( );

    // Initialize server
    std::string bindToAddress = clientCfg.getProperty ( "innerServerAddress" );
    std::uint16_t bindToPort = clientCfg.getIntProperty ( "innerServerPort" );
    
    _server = std::unique_ptr < remolonUtil::RawServer > ( new remolonUtil::RawServer ( bindToAddress, 
                                                                                        bindToPort ) );
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

  void Node::shutDown ( )
  {
    _client.get ( )->close ( );
    _server.get ( )->shutDown ( );
    std::cout << "ReMoLON shut down" << std::endl;
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