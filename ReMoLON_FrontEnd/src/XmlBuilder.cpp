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
