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
 
#ifndef __REMOLON_FRONTEND_INSTANCESERVER_H__
#define __REMOLON_FRONTEND_INSTANCESERVER_H__

#include <memory>
#include <thread>
#include <vector>

#include "ReMoLON_Util/Server.h"

#define REMOLON_NO_SSL_

namespace remolonFrontend
{
	class SessionManagerServer
	{
		public:
      static SessionManagerServer & getInstance ( );

			~SessionManagerServer ( );

			void start ( );
			void shutDown ( );

    private:
      SessionManagerServer ( );

			static SessionManagerServer _INSTANCE;
      
#ifdef REMOLON_NO_SSL_
			std::unique_ptr < remolonUtil::RawServer > _server;
#else
      std::unique_ptr < remolonUtil::SecureServer > _server;
#endif
	};
}

#endif
