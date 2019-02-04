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
		std::cout << "Usage ./rr-frontend [port]" << std::endl;
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
