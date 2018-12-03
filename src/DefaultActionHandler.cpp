#include "DefaultActionHandler.h"

#include <fstream>

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/StreamCopier.h>

#include "UserManager.h"

namespace rrfrontend
{
	std::string DefaultActionHandler::getAction()
	{
		return "";
	}

	void DefaultActionHandler::handleAction(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response)
	{
		const Poco::Net::SocketAddress & sAddr = request.clientAddress();
		Poco::Net::IPAddress ipAddr = sAddr.host();
		std::string ipAddrStr = ipAddr.toString();

		response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);

		if(!UserManager::getInstance().isUserLoggedIn(ipAddrStr))
		{
			std::ifstream file("auth.html", std::ios::binary);
			response.setContentType("text/html");
			Poco::StreamCopier::copyStream(file, response.send());
		}
		else
		{
			std::ifstream file("index.html", std::ios::binary);
			response.setContentType("text/html");
			Poco::StreamCopier::copyStream(file, response.send());
		}
	}
}
