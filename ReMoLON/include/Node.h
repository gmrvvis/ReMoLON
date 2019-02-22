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
      void shutDown ( );

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