#include "frontendserverpackets/RequestNodeInfo.h"

#include "frontendclientpackets/NodeInfo.h"
#include "Node.h"

#include <iostream>

namespace remolon
{
  namespace frontendserverpackets
  {
    char RequestNodeInfo::getOpcode ( )
    {
      return 0x00;
    }

    void RequestNodeInfo::readImpl ( )
    {

    }

    void RequestNodeInfo::executePacketAction ( )
    {
      remolonUtil::SecureClient &
      clnt = Node::getInstance ( ).getFrontendClient ( );

      remolonUtil::SendablePacketPtr 
      nodeInfo = std::make_unique < frontendclientpackets::NodeInfo > ( );

      clnt.sendPacket ( nodeInfo );
    }
  }
}