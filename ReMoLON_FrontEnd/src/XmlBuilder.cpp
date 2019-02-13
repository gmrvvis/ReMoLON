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
 
#include "XmlBuilder.h"

#include "SessionTracker.h"

namespace remolonFrontend
{
	std::string XMLBuildSessionList ( User & usr_ )
	{
		std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><sessionlist>";

		auto list = SessionTracker::getInstance ( ).getUserSessions ( usr_ );

		auto it = list.cbegin ( );
		for( ; it != list.cend ( ); it++ )
		{
			xml += "<session>";
			xml += "<name>" + it->first + "</name>";
			xml += "<url>http://" + it->second._nodePublicAddress + ":" + std::to_string ( it->second._nodePort ) + "/index.html</url>";
      xml += "<status>" + std::to_string ((int) it->second._status ) + "</status>";
			xml += "</session>";
		}

		xml += "</sessionlist>";

		return xml;
	}
}
