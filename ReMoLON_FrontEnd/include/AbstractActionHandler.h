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
 
#ifndef __REMOLON_FRONTEND_ABSTRACTACTIONHANDLER_H__
#define __REMOLON_FRONTEND_ABSTRACTACTIONHANDLER_H__

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <memory>

namespace remolonFrontend
{
	class AbstractActionHandler
	{
		public:
			virtual std::string getAction ( ) = 0;
			virtual void handleAction ( Poco::Net::HTTPServerRequest & request_, 
                                  Poco::Net::HTTPServerResponse & response_ ) = 0;
	};

	typedef std::unique_ptr< AbstractActionHandler > AbstractActionHandlerPtr;
}

#endif
