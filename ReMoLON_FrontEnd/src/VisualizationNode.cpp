#include "VisualizationNode.h"

namespace remolonFrontend
{
  VisualizationNode::VisualizationNode ( )
   : _con ( nullptr )
  {
  }

  void VisualizationNode::setConnection ( remolonUtil::Connection * con_ )
  {
    _con = con_;
    if ( _con )
    {
      _address = _con->peerAddress ( ).toString ( );
    }
  }

  void VisualizationNode::setPublicAddress ( const std::string & publicAddress_ )
  {
    _publicAddress = publicAddress_;
  }

  const std::string & VisualizationNode::getAddress ( )
  {
    return _address; 
  }

  const std::string & VisualizationNode::getPublicAddress ( )
  {
    return _publicAddress;
  }

  unsigned int VisualizationNode::getStreamSlotsCount ( )
  {
    return _totalSlots;
  }

  unsigned int VisualizationNode::getAvailableStreamSlotsCount ( )
  {
    return _totalSlots - _usedSlots;
  }

  void VisualizationNode::setTotalSlots ( unsigned int ts_ )
  {
    _totalSlots = ts_;
  }

  void VisualizationNode::setUsedSlots ( unsigned int us_ )
  {
    _usedSlots = us_;
  }

  bool VisualizationNode::tryReserveSlot ( unsigned int & destPort_ )
  {
    std::unique_lock < std::mutex > lock ( _nodeLock );

    if ( getAvailableStreamSlotsCount ( ) > 0 )
    {
      auto it = _slotTracker.begin ( );
      for( ; it != _slotTracker.end ( ); it++ )
      {
        if ( ! it->second )
        {
          destPort_ = it->first;
          it->second = true;
          _usedSlots++;
          return true;
        }
      }
    }
     
    return false;
  }

  void VisualizationNode::releaseSlot ( unsigned int port_ )
  {
    std::unique_lock < std::mutex > lock ( _nodeLock );

    if( _usedSlots > 0 )
    {
      auto it = _slotTracker.find ( port_ );
      if ( it != _slotTracker.end ( ) )
      {
        it->second = false;
      }
    }
  }

  void VisualizationNode::addAvailablePort ( unsigned int port_, bool used_ )
  {
    std::unique_lock < std::mutex > lock ( _nodeLock );
    _slotTracker [ port_ ] = used_;
  }

  void VisualizationNode::sendPacket ( remolonUtil::SendablePacketPtr & packet )
  {
    if ( _con )
    {
      _con->sendPacket ( packet );
    }
  }
}