#ifndef __REMOLON_FRONTEND_VISUALIZATIONNODE_H__
#define __REMOLON_FRONTEND_VISUALIZATIONNODE_H__

#include <vector>
#include <unordered_map>
#include <string>
#include <mutex>
#include <memory>

#include "ReMoLON_Util/Connection.h"

namespace remolonFrontend
{
  class VisualizationNode
  {
    public:
      VisualizationNode ( );

      void setConnection ( remolonUtil::Connection * con_ );
      void setPublicAddress ( const std::string & publicAddress_ );

      const std::string & getAddress ( );
      const std::string & getPublicAddress ( );

      unsigned int getStreamSlotsCount ( );
      unsigned int getAvailableStreamSlotsCount ( );

      void setTotalSlots ( unsigned int ts_ );
      void setUsedSlots ( unsigned int us_ );

      bool tryReserveSlot ( unsigned int & destPort_ );

      void releaseSlot ( unsigned int port_ );

      void addAvailablePort ( unsigned int port_, bool used_ = false );

      void sendPacket ( remolonUtil::SendablePacketPtr & packet );

    private:
      remolonUtil::Connection * _con;
      std::string _address;
      std::string _publicAddress;
      unsigned int _totalSlots;
      unsigned int _usedSlots;

      std::unordered_map < unsigned int, bool > _slotTracker;

      std::mutex _nodeLock;
  };
}

#endif