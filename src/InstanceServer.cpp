#include "InstanceServer.h"

#include <Poco/Thread.h>

#include <iostream>

namespace rrfrontend
{
	InstanceConnection::InstanceConnection(const Poco::Net::StreamSocket & s)
		: TCPServerConnection(s)
	{
	}

	void InstanceConnection::run()
	{
		Poco::Net::StreamSocket & s = socket();
	
		try
		{	
			int received;
			do
			{
				char buf[0xff];
				received = s.receiveBytes(buf, 0xff);
			
				if(received > 0)
				{
					std::string packet (buf, received);

					std::cout << "Received " << packet << std::endl;
					
					if(packet.find("init") == 0)
					{
						int firstUS = packet.find(";");
						int secondUS = packet.find(";", firstUS + 1);

						if(firstUS != std::string::npos && secondUS != std::string::npos)
						{
							std::string user = packet.substr(firstUS + 1, secondUS - (firstUS + 1));
							std::string session = packet.substr(secondUS + 1, packet.length() - (secondUS + 1));

							std::cout << "Initialized connection for session " << session << " from user " << user << std::endl;
						}
					}

					std::unique_lock<std::mutex> lock(mtx);
			
					while(!sendPackets.empty())
					{
						std::string & packet = sendPackets.front();

						int sentBytes = s.sendBytes(packet.data(), (int) packet.size());
						if(sentBytes != packet.size())
						{
							std::cerr << "Could not send all bytes to connection" << std::endl;
						}

						sendPackets.pop();
					}
					
					lock.unlock();

					Poco::Thread::sleep(50);

				}
			}
			while(received > 0);
		}
		catch(Poco::Exception & e)
		{
			std::cerr << "Error in connection: " << e.displayText() << std::endl;
		}
	}

	void InstanceConnection::sendPacket(const std::string & packet)
	{
		std::unique_lock<std::mutex> lock(mtx);

		sendPackets.push(packet);

		lock.unlock();
	}

	// =======================================================================

	InstanceServer::InstanceServer()
	{

	}

	InstanceServer::~InstanceServer()
	{

	}

	void InstanceServer::start()
	{
		socket = std::make_unique<Poco::Net::SecureServerSocket>(6000);
		server = std::make_unique<Poco::Net::TCPServer>(new Poco::Net::TCPServerConnectionFactoryImpl<InstanceConnection>(), *(socket.get()));
		server.get()->start();
	}

	void InstanceServer::shutDown()
	{
		server.get()->stop();
	}
}
