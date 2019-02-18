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
 
#include <signal.h>
#include <iostream>

#include "Node.h"
#include "SessionManager.h"

#include "frontendclientpackets/NodeInfo.h"

void signalHandler ( int signal )
{
  std::cout << "Received sigaction " << signal << std::endl;
  remolon::SessionManager::getInstance ( ).finishAllSessions ( );
  remolon::Node::getInstance ( ).shutDown ( );
}

//###int main ( int argc, char ** argv )
int main ( void )
{
  using namespace remolon;

  struct sigaction sAct;
  sAct.sa_handler = signalHandler;
  sigemptyset ( &sAct.sa_mask );
  sAct.sa_flags = 0;
  sigaction ( SIGINT , &sAct, 0);
  sigaction ( SIGQUIT, &sAct, 0);

  remolon::Node::getInstance ( ).initialize ( "./remolonClientConfig.cfg" );

  remolonUtil::Client * client = remolon::Node::getInstance ( ).getFrontendClient ( );

  remolonUtil::SendablePacketPtr nodeInfo ( new frontendclientpackets::NodeInfo ( ) );
  client->sendPacket ( nodeInfo );

  remolonUtil::AbstractServer * server = remolon::Node::getInstance ( ).getNodeServer ( );
  server->start ( true );

  return 0;
}
