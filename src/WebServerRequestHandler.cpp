#include "WebServerRequestHandler.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <chrono> 

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/StreamCopier.h>

#include "RequestManager.h"

#include "InstanceManager.h"
#include "UserManager.h"

namespace rrfrontend
{
	void WebServerRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
  {
		RequestManager::getInstance().handleRequest(request, response);
/*
		response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);

		std::cout << "Method: " << request.getMethod() << std::endl;

		std::string action = request.getURI();

		if(action.find_first_of("/") == 0)
		{
			action = action.substr(1, action.length());
		}

		std::cout << "URI: " << action << std::endl;

		// Gather client IP
		const Poco::Net::SocketAddress & sAddr = request.clientAddress();
		Poco::Net::IPAddress ipAddr = sAddr.host();
		std::string ipAddrStr = ipAddr.toString();

		// If the current IP does not belong to an authenticated user
		if(!UserManager::getInstance().isUserLoggedIn(ipAddrStr))
		{
			Poco::Net::HTMLForm form (request, request.stream());
			// If no form was sent, or log in data is wrong, redirect to log in page
			if(!form.has("uname") 
				|| !form.has("psw")
				|| !UserManager::getInstance().tryAuthUser(
						ipAddrStr, 
						form.get("uname", ""), 
						form.get("psw", "")))
			{
				std::ifstream file("auth.html", std::ios::binary);
				response.setContentType("text/html");
				Poco::StreamCopier::copyStream(file, response.send());
				return;
			}
			// User is authenticated
			else
			{
				UserData & data = UserManager::getInstance().getUserData(ipAddrStr);

				std::string address = "";
				std::string port = "";

				// If the given user already has a streaming instance running, redirect to it
				if(InstanceManager::getInstance().userHasInstance(data.unixUser))
				{
					const StreamInstance * si = 
						InstanceManager::getInstance().getInstanceForUser(data.unixUser);
					address = si->getAddress();
					port = std::to_string(si->getPort());
				}
				// Otherwise, create a new one and redirect him to it
				else
				{
					const StreamInstance * si =
						InstanceManager::getInstance().createInstance(data.unixUser, "http://193.147.56.247", 8000, "");
					
					address = si->getAddress();
					port = std::to_string(si->getPort());
				}

				response.redirect(address + ":" + port + "/index.html");
				system("pwd");
			}
		}
	*/
  }
}
