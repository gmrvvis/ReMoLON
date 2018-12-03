#include "WebServer.h"
#include "InstanceServer.h"

int main(int argc, char** argv)
{
	//rrfrontend::InstanceServer instServer;
	//instServer.start();

  rrfrontend::WebServer server;
	return server.run(argc, argv);
}
