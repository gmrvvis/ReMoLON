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
 
#ifndef __REMOLON_UTIL_CONFIG_H__
#define __REMOLON_UTIL_CONFIG_H__

#include <vector>
#include <unordered_map>

namespace remolonUtil
{
  class Config
  {
    public:
      Config ( const std::string & configFilePath_  );

      std::string operator [] ( const std::string & key_ ) const;
      std::string getProperty ( const std::string & key_,
                                const std::string & defaultValue_ = "" ) const;
      std::vector < std::string > getPropertyList ( const std::string & key_, const std::string & delimiter_ );
      int getIntProperty ( const std::string & key_, int defaultValue_ = 0 ) const;
      
    private:
      void load ( const std::string & configFilePath_ );
      std::string removeWhiteSpaces ( const std::string & source_ );

      std::unordered_map < std::string, std::string > _properties;
  };
}

#endif