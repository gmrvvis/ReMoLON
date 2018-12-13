#include "WebServer.h"
#include "InstanceServer.h"

#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main(int argc, char** argv)
{
	unsigned int port = 80;
	if(argc < 2)
	{
		std::cout << "Usage ./rr-frontend [port]" << std::endl;
		std::cout << "If no port is specified, 80 will be used by default" << std::endl;
	}
	else
	{
		try
		{
			port = atoi(argv[1]);
		}
		catch(std::runtime_error & re)
		{
			std::cerr << "Could not determine used port from input: " << argv[1] << std::endl;
			return -1;
		}
	}
	//rrfrontend::InstanceServer instServer;
	//instServer.start();

	rrfrontend::WebServer server (port);
	return server.run(argc, argv);
}
