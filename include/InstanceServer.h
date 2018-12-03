#ifndef __RRFRONTEND_INSTANCESERVER_H__
#define __RRFRONTEND_INSTANCESERVER_H__

#include <Poco/Net/SecureServerSocket.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/TCPServerParams.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/TCPServer.h>

#include <queue>
#include <mutex>

#include <memory>

namespace rrfrontend
{
	class InstanceConnection : public Poco::Net::TCPServerConnection
	{
		private:
			std::mutex mtx;
			std::queue<std::string> sendPackets;
		public:
			InstanceConnection(const Poco::Net::StreamSocket & s);
			void run();

			void sendPacket(const std::string & packet);
	};

	class InstanceServer
	{
		private:
			std::unique_ptr<Poco::Net::SecureServerSocket> socket;
			std::unique_ptr<Poco::Net::TCPServer> server;
		public:
			InstanceServer();
			~InstanceServer();

			void start();
			void shutDown();
	};
}

#endif
