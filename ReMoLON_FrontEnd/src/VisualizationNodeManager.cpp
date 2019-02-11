#include "VisualizationNodeManager.h"

#include <vector>
#include <iostream>

namespace remolonFrontend
{
  // ======================================

  VisualizationNodeManager VisualizationNodeManager::_INSTANCE;

  VisualizationNodeManager & VisualizationNodeManager::getInstance ( )
  {
    return _INSTANCE;
  }

  void VisualizationNodeManager::setLoadBalanceEnabled ( bool val )
  {
    _enableLoadBalance = val;
  }

  VisualizationNode * VisualizationNodeManager::registerOrGetNode ( remolonUtil::Connection * con_ )
  {
    std::string address = con_->clientAddress ( );
    
    VisualizationNode * node = getNode ( address );
    if ( node != nullptr )
    {
      node->setConnection ( con_ );
      return node;
    }
    else
    {
      VisualizationNode & newNode = _nodeList [ address ];
      newNode.setConnection ( con_ );
      return &newNode;
    }
  }

  VisualizationNode * VisualizationNodeManager::getNode ( const std::string & address_ )
  {
    auto it = _nodeList.find ( address_ );
    if ( it != _nodeList.end ( ) )
    {
      return &(it->second);
    }

    return nullptr;
  }

  VisualizationNode * VisualizationNodeManager::findBestNode ( )
  {
    std::unique_lock < std::mutex > lock ( _mtx );

    if ( _enableLoadBalance )
    {
      return findBest ( );
    }
    else
    {
      return find ( );
    }
  }

  VisualizationNode * VisualizationNodeManager::findBest ( )
  {
    auto it = _nodeList.begin ( );
    VisualizationNode * bestCandidate = nullptr;
    unsigned int mostAvailable = 0;

    for ( ; it != _nodeList.end ( ); it++ )
    {
      VisualizationNode & visNode = it->second;  

      if ( visNode.getAvailableStreamSlotsCount ( ) > mostAvailable )
      {
        mostAvailable = visNode.getAvailableStreamSlotsCount ( );
        bestCandidate = &visNode;
      }
    }

    return bestCandidate;
  }

  VisualizationNode * VisualizationNodeManager::find ( )
  {
    auto it = _nodeList.begin ( );
    for ( ; it != _nodeList.end ( ); it++ )
    {
      VisualizationNode & visNode = it->second;  

      if ( visNode.getAvailableStreamSlotsCount ( ) > 0 )
      {
        return &visNode;
      } 
    }

    return nullptr;
  }
}