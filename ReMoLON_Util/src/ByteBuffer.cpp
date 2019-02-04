#include "ReMoLON_Util/ByteBuffer.h"

#include <cstring>
#include <stdexcept>

namespace remolonUtil
{
  ByteBuffer::ByteBuffer ( )
   : ByteBuffer ( 65535 )
  {
  }

  ByteBuffer::ByteBuffer ( const std::size_t & size_ )
   : _buf ( nullptr )
   , _used ( 0 )
   , _read ( 0 )
   , _size ( 0 )
  {
    resize ( size_ );
  }

  ByteBuffer::~ByteBuffer ( )
  {
    if( _buf != nullptr )
    {
      delete[] _buf;
    }
  }

  void ByteBuffer::resize ( const std::size_t & newSize_ )
  {
    char * temp = nullptr;
    std::size_t oldUsed = _used;
    if ( _buf != nullptr )
    {
      if( oldUsed > 0)
      {
        temp = new char [ _size ];
        memcpy ( temp, _buf, oldUsed );
      }

      delete[] _buf;
    }

    _size = newSize_;
    _used = 0;
    _read = 0;

    if( newSize_ > 0 )
    {
      _buf = new char [ _size ];

      if( oldUsed > 0 )
      {
        memcpy ( _buf, temp, oldUsed );
        delete[] temp;
      }
    }
    else
    {
      _buf = nullptr;
    }
  }

  char * ByteBuffer::getData ( )
  {
    return _buf;
  }

  char * ByteBuffer::getAppendPtr ( )
  {
    return ( _buf + _used);
  }

  char * ByteBuffer::getReadPtr ( )
  {
    return ( _buf + _read);
  }

  bool ByteBuffer::appendContent ( ByteBuffer & other )
  {
    if ( other.getUsedSize ( ) < getRemainingWriteSize ( ) )
    {
      memcpy ( getAppendPtr ( ), other._buf, other.getUsedSize ( ) );
      _used += other.getUsedSize ( );
      return true;
    }
    
    return false;
  }

  const std::size_t & ByteBuffer::getSize ( )
  {
    return _size;
  }

  const std::size_t & ByteBuffer::getUsedSize ( )
  {
    return _used;
  }

  std::size_t ByteBuffer::getWritePos ( )
  {
    return _used;
  }

  std::size_t ByteBuffer::getReadPos ( )
  {
    return _read;
  }

  std::size_t ByteBuffer::getRemainingWriteSize ( )
  {
    return _size - _used;
  }

  std::size_t ByteBuffer::getRemainingReadSize ( )
  {
    return _used - _read;
  }

  void ByteBuffer::setReadPos ( const std::size_t & rp_ )
  {
    _read = rp_;
  }

  void ByteBuffer::setWritePos ( const std::size_t & wp_ )
  {
    _used = wp_;
  }

  void ByteBuffer::compact ( )
  {
    std::size_t usedLen = getRemainingReadSize ( );
    std::memcpy ( _buf, _buf + _read, usedLen );
    setReadPos ( 0 );
    setWritePos ( usedLen );
  }

  void ByteBuffer::rewind ( )
  {
    setReadPos ( 0 );
  }

  void ByteBuffer::reset ( )
  {
    _used = _read = 0;
  }

  void ByteBuffer::writeChar ( char c_ )
  {
    if ( sizeof( c_ ) + _used < _size)
    {
      _buf[_used] = c_;
      _used += sizeof ( c_ );
    }
  }

  void ByteBuffer::writeShort ( int16_t s_ )
  {
    if ( sizeof( int16_t ) + _used < _size)
    {
      //std::memcpy ( getAppendPtr ( ), &s_, sizeof ( s_ ));

      char * buf = getAppendPtr ( );
      buf[ 0 ] = s_ & 0xff;
      buf[ 1 ] = ( s_ >> 8 ) & 0xff;

      _used += sizeof ( s_ );
    }
  }

  void ByteBuffer::writeInt ( int32_t i_ )
  {
    if ( sizeof( int32_t ) + _used < _size)
    {
      //std::memcpy( getAppendPtr ( ), &i_, sizeof ( i_ ));
      char * buf = getAppendPtr ( );
      buf [ 0 ] = i_ & 0xff;
      buf [ 1 ] = ( i_ >> 8 ) & 0xff;
      buf [ 2 ] = ( i_ >> 16 ) & 0xff;
      buf [ 3 ] = ( i_ >> 24 ) & 0xff;

      _used += sizeof ( i_ );
    }
  }

  
  void ByteBuffer::writeLong ( int64_t l_ )
  {
    if ( sizeof( int64_t ) + _used < _size)
    {
      //std::memcpy( getAppendPtr ( ), &l_, sizeof ( l_ ));
      char * buf = getAppendPtr ( );
      buf [ 0 ] = l_ & 0xff;
      buf [ 1 ] = ( l_ >> 8 ) & 0xff;
      buf [ 2 ] = ( l_ >> 16 ) & 0xff;
      buf [ 3 ] = ( l_ >> 24 ) & 0xff;
      buf [ 4 ] = ( l_ >> 32 ) & 0xff;
      buf [ 5 ] = ( l_ >> 40 ) & 0xff;
      buf [ 6 ] = ( l_ >> 48 ) & 0xff;
      buf [ 7 ] = ( l_ >> 56 ) & 0xff;

      _used += sizeof ( l_ );
    }
  }
  

  void ByteBuffer::writeFloat ( float f_ )
  {
    if ( sizeof ( float ) + _used < _size )
    {
      std::memcpy( getAppendPtr ( ), &f_, sizeof ( f_ ));
      _used += sizeof ( f_ );
    }
  }

  void ByteBuffer::writeDouble ( double d_ )
  {
    if ( sizeof ( double ) + _used < _size )
    {
      std::memcpy( getAppendPtr ( ), &d_, sizeof ( d_ ));
      _used += sizeof ( d_ );
    }
  }

  void ByteBuffer::writeString ( const std::string & str_ )
  {
    if( str_.size ( ) + sizeof ( short ) + _used < _size )
    {
      unsigned short strLen = (short)str_.length ( );
      writeShort ( strLen );
      std::memcpy ( getAppendPtr ( ), str_.data ( ), str_.size ( ));
      _used += str_.size ( );
    }
  }

  char ByteBuffer::readChar ( )
  {
    if ( _read + sizeof ( char ) > _used )
    {
      throw std::runtime_error ( "ByteBuffer: attempted to read beyond written limit" );
      return 0;
    }

    char result = _buf [ _read ];
    _read += sizeof (char );
    return result;
  }

  int16_t ByteBuffer::readShort ( )
  {
    if ( _read + sizeof ( int16_t ) > _used )
    {
      throw std::runtime_error ( "ByteBuffer: attempted to read beyond written limit ");
      return 0;
    }

    char * buf = getReadPtr ( );
    int16_t result = ( (buf [ 0 ] & 0xff) | (( buf [ 1 ] << 8 ) & 0xff00) );
    _read += sizeof ( int16_t );
    return result;
  }

  int32_t ByteBuffer::readInt ( )
  {
    if ( _read + sizeof ( int32_t ) > _used )
    {
      throw std::runtime_error ( "ByteBuffer: attempted to read beyond written limit ");
      return 0;
    }

    char * buf = getReadPtr ( );
    int32_t result = ( (buf[0] & 0xff) 
                      | ((buf[1] << 8) & 0xff00) 
                      | ((buf[2] << 16) & 0xff0000) 
                      | ((buf[3] << 24) & 0xff000000) );
    _read += sizeof ( int32_t );
    return result;
  }
  
  int64_t ByteBuffer::readLong ( )
  {
    if ( _read + sizeof ( int64_t ) > _used )
    {
      throw std::runtime_error ( "ByteBuffer: attempted to read beyond written limit ");
      return 0; 
    }

    char * buf = getReadPtr ( );
    int64_t result = ( (buf[0] & 0xff) 
                      | ((buf[1] << 8) & 0xff00) 
                      | ((buf[2] << 16) & 0xff0000) 
                      | ((buf[3] << 24) & 0xff000000)
                      | (((int64_t)buf[4] << 32) & 0xff00000000)
                      | (((int64_t)buf[5] << 40) & 0xff0000000000)
                      | (((int64_t)buf[6] << 48) & 0xff000000000000)
                      | (((int64_t)buf[7] << 56) & 0xff00000000000000));
    // Requires explicit cast from 32 shift offset onwards because bitwise operators
    // implicitly cast them into 32 bit integers
    _read += sizeof ( int64_t );
    return result;
  }

  float ByteBuffer::readFloat ( )
  {
    if ( _read + sizeof ( float ) > _used )
    {
      throw std::runtime_error ( "ByteBuffer: attempted to read beyond written limit ");
      return 0.0f;
    }

    float result;
    memcpy(&result, getReadPtr ( ), sizeof ( float ));
    _read += sizeof ( float );
    return result;
  }

  double ByteBuffer::readDouble ( )
  {
    if ( _read + sizeof ( double ) > _used )
    {
      throw std::runtime_error ( "ByteBuffer: attempted to read beyond written limit ");
      return 0.0;
    }

    double result;
    memcpy(&result, getReadPtr ( ), sizeof ( double ));
    _read += sizeof ( double );
    return result;
  }

  std::string ByteBuffer::readString ( )
  {
    // Max string size: 65535
    unsigned short strSize = readShort ( );

    if ( strSize + _read > _used)
    {
      throw std::runtime_error ( "ByteBuffer: attempted to read beyond written limit ");
      return "";
    }

    std::string result ( getReadPtr ( ), strSize);
    _read += strSize;
    return result;
  }
}