#include "SessionListActionHandler.h"

#include <fstream>

#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/IPAddress.h>

#include "UserManager.h"
#include "XMLBuilder.h"
#include "InstanceManager.h"

namespace rrfrontend
{
	std::string SessionListActionHandler::getAction()
	{
		return "RefreshSessionList";
	}

	void SessionListActionHandler::handleAction(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response)
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
			UserData & data = UserManager::getInstance().getUserData(ipAddrStr);
			const UserSessionList & userSessionList = InstanceManager::getInstance().getUserInstances(data.unixUser);
			//std::string test = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><sessionlist><session><name>TestSession</name><url>http://localhost:8000/index.html</url></session></sessionlist>";
			std::string result = XMLBuildSessionList(userSessionList);
			response.send() << result;
		}
	}
}
