#ifndef __REMOLON_NODE_H__
#define __REMOLON_NODE_H__

#include <string>
#include <memory>
#include <unordered_map>

#include "ReMoLON_Util/SecureClient.h"
#include "ReMoLON_Util/SecureServer.h"

namespace remolon
{
  class Node
  {
    public:
      static Node & getInstance ( );

      void initialize ( const std::string & configFilePath_ );

      remolonUtil::SecureClient & getFrontendClient ( );
      remolonUtil::SecureServer & getNodeServer ( );

    private:
      static Node _INSTANCE;

      // Client to the frontend server
      std::unique_ptr < remolonUtil::SecureClient > _client;
      // Server to the streaming sessions
      std::unique_ptr < remolonUtil::SecureServer > _server;
  };
}

#endif