#include "ReMoLON_Util/Config.h"

#include <fstream>
#include <iostream>

namespace remolonUtil
{
  Config::Config ( const std::string & ConfigFilePath_ )
  {
    load ( ConfigFilePath_ );
  }

  std::string Config::operator[] ( const std::string & key_ ) const
  {
    return getProperty ( key_ );
  }

  std::string Config::getProperty ( const std::string & key_, 
                                    const std::string & defaultValue_ ) const
  {
    auto it = _properties.find ( key_ );
    if( it != _properties.end ( ) )
    {
      return it->second;
    }

    return defaultValue_;
  }

  std::vector < std::string > Config::getPropertyList ( const std::string & key_,
                                                        const std::string & delimiter_ )
  {
    std::vector < std::string > result;

    auto it = _properties.find ( key_ );
    if( it != _properties.end ( ) )
    {
      std::string content = it->second;

      std::size_t charPos = 0;
      std::size_t prevPos = 0;

      do
      {
        charPos = content.find_first_of ( delimiter_, charPos );
        if ( charPos != std::string::npos )
        {
          std::string subStr = content.substr ( prevPos, charPos - prevPos );
          result.push_back ( subStr );
          charPos++;
          prevPos = charPos;
        }
        else
        {
          std::string lastPiece = content.substr ( prevPos, content.length( ) - prevPos );
          result.push_back ( lastPiece );
        }
      }
      while ( charPos != std::string::npos );
    }

    return result;
  }

  int Config::getIntProperty ( const std::string & key_, int defaultValue_ ) const
  {
    const std::string & val = getProperty ( key_ );
    if ( val != "" )
    {
      return std::stoi ( val );
    }

    return defaultValue_;
  }

  void Config::load ( const std::string & ConfigFilePath_ )
  {
    std::ifstream fis ( ConfigFilePath_.c_str ( ), std::ifstream::in );
    if ( fis )
    {
      std::string line;
      while ( std::getline ( fis, line ) )
      {
        line = removeWhiteSpaces ( line );

        if ( line.empty ( ) )
        {
          continue;
        }

        if ( line.find ( "#" ) == 0 )
        {
          continue;
        }

        std::size_t equalSim = line.find_first_of ( "=" );

        if ( equalSim != std::string::npos )
        {
          std::string propertyName = line.substr ( 0, equalSim );
          std::string propertyValue = line.substr ( equalSim + 1, line.length ( ) );
          _properties [ propertyName ] = propertyValue;
        }
      }

      fis.close ( ); 
    }
    else
    {
      std::cerr << "error: could not read Configuration file " << ConfigFilePath_ << std::endl;
    }
  }

  std::string Config::removeWhiteSpaces ( const std::string & source_ )
  {
    std::string result = "";
    auto it = source_.begin ( );
    for ( ; it != source_.end ( ); it++ )
    {
      if ( *it != ' ' )
      {
        result.append ( it, it + 1 );
      }
    }

    return result;
  }
}