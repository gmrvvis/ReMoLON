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

#include "RequestManager.h"

namespace remolonFrontend
{
	RequestManager RequestManager::_INSTANCE;

	RequestManager & RequestManager::getInstance ( )
	{
		return _INSTANCE;
	}

	RequestManager::RequestManager ( )
	{
	}

	RequestManager::~RequestManager ( )
	{
	}

	void RequestManager::handleRequest ( Poco::Net::HTTPServerRequest & request_, 
                                       Poco::Net::HTTPServerResponse & response_ )
	{
		std::string action = request_.getURI ( );

		if ( action.find_first_of ( "/" ) == 0 )
		{
			action = action.substr ( 1, action.length ( ) );
		}

		auto it = _actionHandlers.find ( action );

		if ( it == _actionHandlers.end ( ) )
		{
			response_.setStatus ( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
		}	
		else
		{
			AbstractActionHandlerPtr & handler = it->second;
			handler.get ( )->handleAction ( request_, response_ );
		}
	}
}	
