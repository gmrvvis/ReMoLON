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
 
#ifndef __REMOLON_FRONTEND_WEBSERVER_H__
#define __REMOLON_FRONTEND_WEBSERVER_H__

#include <vector>
#include <string>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

namespace remolonFrontend
{
	class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
	{
	  public:
		  virtual Poco::Net::HTTPRequestHandler * createRequestHandler ( const Poco::Net::HTTPServerRequest & );
	};

	class WebServer : public Poco::Util::ServerApplication
	{
	public:
		WebServer ( unsigned int port_ );
  private:
		unsigned int _port;
	protected:
		int main ( const std::vector < std::string > & );
	};
}

#endif
