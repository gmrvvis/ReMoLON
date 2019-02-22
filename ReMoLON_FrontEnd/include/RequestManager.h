/*
 * Copyright (c) 2019 CCS/UPM - GMRV/URJC.
 *
 * Authors: Nadir Román Guerrero <nadir.roman@urjc.es>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
 
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

				AbstractActionHandlerPtr newHandler ( new T ( ) );//= std:: < T > ( );
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
