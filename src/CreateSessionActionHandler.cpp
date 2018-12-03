#include "CreateSessionActionHandler.h"

#include <fstream>

#include <Poco/Net/HTMLForm.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/IPAddress.h>

#include "UserManager.h"
#include "InstanceManager.h"

namespace rrfrontend
{
	std::string CreateSessionActionHandler::getAction()
	{
		return "CreateSession";
	}

	void CreateSessionActionHandler::handleAction(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response)
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

			Poco::Net::HTMLForm form (request, request.stream());
			std::string newSessionName = form.get("session_name", "");

			if(newSessionName == "")
			{
				response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
				response.send() << "No session name specified";
			}
			else
			{
				int result;
				const StreamInstance * si =
						InstanceManager::getInstance().createInstance(newSessionName, data.unixUser, "193.147.56.247", 8000, "", result);

				switch(result)
				{
					case InstanceManager::CREATION_FAILED_DUPLICATE_NAME:
						response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
						response.send() << "A session with the name " +newSessionName + " already exists";
						break;
					case InstanceManager::CREATION_SUCCESSFUL:
						response.send() << "";
						break;
				}
			}
		}
	}
}
