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

  remolonUtil::SecureClient & client = remolon::Node::getInstance ( ).getFrontendClient ( );

  remolonUtil::SendablePacketPtr nodeInfo = std::make_unique < frontendclientpackets::NodeInfo > ( );
  client.sendPacket ( nodeInfo );

  std::unique_lock < std::mutex > l ( m );
  cv.wait ( l );

  std::cout << "IM OUT <<<<<<<<<<<<<<<<<" << std::endl;

  return 0;
}