#include "clientpackets/NodeInfo.h"

#include "ReMoLON_Util/Connection.h"
#include "VisualizationNodeManager.h"

#include <iostream>

namespace remolonFrontend
{
  namespace clientpackets
  {
    char NodeInfo::getOpcode ( )
    {
      return 0x00;
    }

    void NodeInfo::readImpl ( )
    {
      VisualizationNode * node = VisualizationNodeManager::getInstance ( ).registerOrGetNode ( getConnection ( ) );

      std::string publicAddr = readString ( );
      node->setPublicAddress ( publicAddr );

      const int totalSlots = readInt ( );
      node->setTotalSlots ( totalSlots );

      std::cout << "New visualization node registered on address " << publicAddr << std::endl;

      int used = 0;
      for ( int i = 0; i <  totalSlots; i++ )
      {
        int slotPort = readInt ( );
        bool usedPort = readInt ( ) > 0;

        std::cout << "Port " << slotPort << " used? " << usedPort << std::endl;

        node->addAvailablePort ( slotPort, usedPort );

        if ( usedPort )
        {
          used++;
        }
      }

      node->setUsedSlots ( used );
    }

    void NodeInfo::executePacketAction ( )
    {

    }
  }
}