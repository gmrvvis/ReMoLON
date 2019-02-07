#ifndef __REMOLON_UTIL_PACKET_H__
#define __REMOLON_UTIL_PACKET_H__

#include <string>
#include <memory>

#include "ReMoLON_Util/ByteBuffer.h"
#include "ReMoLON_Util/Runnable.h"

namespace remolonUtil
{
  class Connection;

  class Packet : public Runnable
  {
    public:
      virtual ~Packet ( ) = default;

      void setConnection ( Connection * con_ );
      Connection * getConnection ( );
      
      void setBuffer ( ByteBuffer * buf_ );

      virtual char getOpcode ( ) = 0;

      virtual void run ( );
    protected:
      Connection * _con;
      ByteBuffer * _buf;
  };

  typedef std::unique_ptr < Packet > PacketPtr;

  class SendablePacket : public Packet
  {
    public:
      virtual ~SendablePacket ( ) = default;

      void writeChar ( char c_ );
      void writeShort ( short s_ );
      void writeInt ( int i_ );
      void writeLong ( long l_ );
      void writeFloat ( float f_ );
      void writeDouble ( double d_ );
      void writeString ( const std::string & s_ );

      void writePacket ( );

      virtual void writeImpl ( ) = 0;
  };

  typedef std::unique_ptr < SendablePacket > SendablePacketPtr;

  class ReceivablePacket : public Packet
  {
    public:
      virtual ~ReceivablePacket ( ) = default;

      char readChar ( );
      short readShort ( );
      int readInt ( );
      long readLong ( );
      float readFloat ( );
      double readDouble ( );
      std::string readString ( );

      void run ( );

      virtual void readImpl ( ) = 0;
      virtual void executePacketAction ( ) = 0;
  };

  typedef std::unique_ptr < ReceivablePacket > ReceivablePacketPtr;
}

#endif