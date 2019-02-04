#ifndef __REMOLON_UTIL_BYTEBUFFER_H__
#define __REMOLON_UTIL_BYTEBUFFER_H__

#include <cstddef>
#include <string>

namespace remolonUtil
{
  class ByteBuffer
  {
    public:
      ByteBuffer ( );
      ByteBuffer ( const std::size_t & size_ );
      ~ByteBuffer ( );

      void resize ( const std::size_t & newSize_ );

      char * getData ( );
      char * getAppendPtr ( );
      char * getReadPtr ( );

      bool appendContent ( ByteBuffer & other );

      const std::size_t & getSize ( );
      const std::size_t & getUsedSize ( );
      std::size_t getWritePos ( );
      std::size_t getReadPos ( );
      std::size_t getRemainingWriteSize ( );
      std::size_t getRemainingReadSize ( );
      void setWritePos ( const std::size_t & wp_ );
      void setReadPos ( const std::size_t & rp_ );

      void compact ( );
      void rewind ( );
      void reset ( );

      void writeChar ( char c_ );
      void writeShort ( int16_t s_ );
      void writeInt ( int32_t i_ );
      void writeLong ( int64_t l_ );
      void writeFloat ( float f_ );
      void writeDouble ( double d_ );
      void writeString ( const std::string & str_ );

      char readChar ( );
      int16_t readShort ( );
      int32_t readInt ( );
      int64_t readLong ( ); 
      float readFloat ( );
      double readDouble ( );
      std::string readString ( );

    private:
      char * _buf;

      std::size_t _size;
      std::size_t _used;
      std::size_t _read;
  };
}

#endif