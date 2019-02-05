#include "clientpackets/NodeInfo.h"

#include "ReMoLON_Util/Connection.h"
#include "VisualizationNodeManager.h"

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

      int used = 0;
      for ( int i = 0; i <  totalSlots; i++ )
      {
        int slotPort = readInt ( );
        bool usedPort = readInt ( ) > 0;

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