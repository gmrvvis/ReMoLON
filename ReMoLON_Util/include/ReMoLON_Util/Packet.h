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