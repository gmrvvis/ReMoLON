#ifndef __REMOLON_NODE_H__
#define __REMOLON_NODE_H__

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "ReMoLON_Util/SecureClient.h"
#include "ReMoLON_Util/Server.h"

namespace remolon
{
  class Node
  {
    public:
      static Node & getInstance ( );

      void initialize ( const std::string & configFilePath_ );

      remolonUtil::SecureClient & getFrontendClient ( );
      remolonUtil::Server & getNodeServer ( );

      const std::string & getRemotooBinaryDir ( );
      const std::string & getRemolonXinitrcBinaryDir ( );

    private:
      static Node _INSTANCE;

      std::string _remotooBinDir;
      std::string _remolonXinitrcDir;

      // Client to the frontend server
      std::unique_ptr < remolonUtil::SecureClient > _client;
      // Server to the streaming sessions
      std::unique_ptr < remolonUtil::Server > _server;
  };
}

#endif