#ifndef __REMOLON_NODE_H__
#define __REMOLON_NODE_H__

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "ReMoLON_Util/Client.h"
#include "ReMoLON_Util/Server.h"

#define REMOLON_NO_SSL_

namespace remolon
{
  class Node
  {
    public:
      static Node & getInstance ( );

      void initialize ( const std::string & configFilePath_ );

      remolonUtil::Client * getFrontendClient ( );
      remolonUtil::AbstractServer * getNodeServer ( );

      const std::string & getRemotooBinaryDir ( );
      const std::string & getRemolonXinitrcBinaryDir ( );

    private:
      static Node _INSTANCE;

      std::string _remotooBinDir;
      std::string _remolonXinitrcDir;

      // Client to the frontend server
#ifdef REMOLON_NO_SSL_
      std::unique_ptr < remolonUtil::RawClient > _client;
#else
      std::unique_ptr < remolonUtil::SecureClient > _client;
#endif
      // Server to the streaming sessions
      std::unique_ptr < remolonUtil::RawServer > _server;
  };
}

#endif