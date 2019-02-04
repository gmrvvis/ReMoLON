#ifndef __REMOLON_FRONTEND_REQUESTMANAGER_H__
#define __REMOLON_FRONTEND_REQUESTMANAGER_H__

#include <unordered_map>
#include <iostream>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "AbstractActionHandler.h"

namespace remolonFrontend
{
	class RequestManager
	{
		public:
			static RequestManager & getInstance ( );
		
			~RequestManager ( );

			template<class T>
			bool registerActionHandler ( )
			{
				if ( !std::is_base_of< AbstractActionHandler, T >::value )
				{
					std::cerr << "Attempted to register a non AbstractActionHandler derived class" << std::endl;
					return false;
				}

				AbstractActionHandlerPtr newHandler = std::make_unique < T > ( );
				AbstractActionHandler * handlerPtr = newHandler.get ( );
				_actionHandlers [ handlerPtr->getAction ( ) ] = std::move ( newHandler );
				return true;
			}

			void handleRequest ( Poco::Net::HTTPServerRequest & request_, 
                           Poco::Net::HTTPServerResponse & response_ );

    private:
      RequestManager ( );

      static RequestManager _INSTANCE;
      std::unordered_map < std::string, AbstractActionHandlerPtr > _actionHandlers;
	};
}

#endif
