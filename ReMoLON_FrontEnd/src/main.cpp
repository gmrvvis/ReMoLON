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

#include "WebServer.h"
#include "SessionManagerServer.h"

#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main ( int argc_, char ** argv_ )
{
	unsigned int port = 80;
	if ( argc_ < 2 )
	{
		std::cout << "Usage ./remolon_frontend_server [port]" << std::endl;
		std::cout << "If no port is specified, 80 will be used by default" << std::endl;
	}
	else
	{
		try
		{
			port = atoi ( argv_ [ 1 ] );
		}
		catch ( std::runtime_error & re )
		{
			std::cerr << "Could not determine used port from input: " << argv_ [ 1 ] << std::endl;
			return -1;
		}
	}
  
	remolonFrontend::SessionManagerServer::getInstance ( ).start( );

	remolonFrontend::WebServer server ( port );
	int retVal = server.run ( argc_, argv_ );

  remolonFrontend::SessionManagerServer::getInstance ( ).shutDown ( );

  return retVal;
}
