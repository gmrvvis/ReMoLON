#ifndef __RRFRONTEND_REQUESTMANAGER_H__
#define __RRFRONTEND_REQUESTMANAGER_H__

#include <unordered_map>
#include <iostream>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "AbstractActionHandler.h"

namespace rrfrontend
{
	class RequestManager
	{
		private:
			static RequestManager INSTANCE;
		public:
			static RequestManager & getInstance();
		private:
			std::unordered_map<std::string, AbstractActionHandlerPtr> actionHandlers;
		private:
			RequestManager();
		public:
			~RequestManager();

			template<class T>
			bool registerActionHandler()
			{
				if(!std::is_base_of<AbstractActionHandler, T>::value)
				{
					std::cerr << "Attempted to register a non AbstractActionHandler derived class" << std::endl;
					return false;
				}

				AbstractActionHandlerPtr newHandler = std::make_unique<T>();
				AbstractActionHandler * handlerPtr = newHandler.get();
				actionHandlers[handlerPtr->getAction()] = std::move(newHandler);
				return true;
			}

			void handleRequest(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response);
	};
}

#endif
