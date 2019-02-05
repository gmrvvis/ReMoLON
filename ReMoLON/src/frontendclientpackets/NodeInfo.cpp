#include "frontendclientpackets/NodeInfo.h"

#include "SessionManager.h"

namespace remolon
{
  namespace frontendclientpackets
  {
    char NodeInfo::getOpcode ( )
    {
      return 0x00;
    }

    void NodeInfo::writeImpl ( )
    {
      const std::string & address = SessionManager::getInstance ( ).getPublicAddress ( );
      writeString ( address );

      const std::unordered_map < uint16_t, bool > & 
      portList = SessionManager::getInstance ( ).getPortList ( );

      const int slots = portList.size ( );
      writeInt ( slots );
    
      auto it = portList.cbegin ( );
      for ( ; it != portList.cend ( ); it++ )
      {
        writeInt ( it->first );
        writeInt ( it->second ? 1 : 0 ); 
      }
    }
  }
}