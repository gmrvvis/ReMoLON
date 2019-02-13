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