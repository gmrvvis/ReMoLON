#ifndef __REMOLON_FRONTEND_VISUALIZATIONNODEMANAGER_H__
#define __REMOLON_FRONTEND_VISUALIZATIONNODEMANAGER_H__

#include <string>
#include <mutex>
#include <unordered_map>

#include "VisualizationNode.h"
#include "ReMoLON_Util/Connection.h"

namespace remolonFrontend
{
  class VisualizationNodeManager
  {
    public:
      static VisualizationNodeManager & getInstance ( );

      VisualizationNode * getNode ( const std::string & address_ );
      VisualizationNode * findBestNode ( );
      VisualizationNode * registerOrGetNode ( remolonUtil::Connection * con_ );

      void setLoadBalanceEnabled ( bool val );

    private:
      VisualizationNode * findBest ( );
      VisualizationNode * find ( );

      static VisualizationNodeManager _INSTANCE;

      // Synchronizes reservation of node slot
      std::mutex _mtx;
      bool _enableLoadBalance;

      std::unordered_map < std::string, VisualizationNode > _nodeList;
  };
}

#endif