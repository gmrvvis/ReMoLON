#include "XMLBuilder.h"

namespace rrfrontend
{
	std::string XMLBuildSessionList(const UserSessionList & list)
	{
		std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><sessionlist>";

		auto it = list.cbegin();
		for(; it != list.cend(); it++)
		{
			xml += "<session>";
			xml += "<name>" + it->first + "</name>";
			xml += "<url>http://" + it->second.get()->address + ":" + std::to_string(it->second.get()->port) + "/index.html</url>";
			xml += "</session>";
		}

		xml += "</sessionlist>";

		return xml;
	}
}
