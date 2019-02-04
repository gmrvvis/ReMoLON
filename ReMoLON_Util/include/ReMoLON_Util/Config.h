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