#include <iostream>
#include <mutex>
#include <condition_variable>

#include "Node.h"

#include "frontendclientpackets/NodeInfo.h"

std::mutex m;
std::condition_variable cv;

int main ( int argc, char ** argv )
{
  using namespace remolon;

  remolon::Node::getInstance ( ).initialize ( "./remolonClientConfig.cfg" );

  remolonUtil::Client * client = remolon::Node::getInstance ( ).getFrontendClient ( );

  remolonUtil::SendablePacketPtr nodeInfo = std::make_unique < frontendclientpackets::NodeInfo > ( );
  client->sendPacket ( nodeInfo );

  remolonUtil::AbstractServer * server = remolon::Node::getInstance ( ).getNodeServer ( );
  server->start ( true );

  return 0;
}