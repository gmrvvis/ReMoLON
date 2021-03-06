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
 
#include "ReMoLON_Util/Packet.h"

#include "ReMoLON_Util/Connection.h"

namespace remolonUtil
{
  void Packet::setConnection ( Connection * con_ )
  {
    _con = con_;
  }

  Connection * Packet::getConnection( )
  {
    return _con;
  }

  void Packet::setBuffer ( ByteBuffer * buf_ )
  {
    _buf = buf_;
  }

  void Packet::run ( )
  {
    
  }

  void SendablePacket::writeChar( char c_ )
  {
    if ( _buf )
    {
      _buf->writeChar ( c_ );
    }
  }

  void SendablePacket::writeShort ( short s_ )
  {
    if ( _buf )
    {
      _buf->writeShort ( s_ );
    }
  }

  void SendablePacket::writeInt ( int i_ )
  {
    if ( _buf )
    {
      _buf->writeInt ( i_ );
    }
  }

  void SendablePacket::writeLong ( long l_ )
  {
    if ( _buf )
    {
      _buf->writeLong ( l_ );
    }
  }

  void SendablePacket::writeFloat ( float f_ )
  {
    if ( _buf )
    {
      _buf->writeFloat ( f_ );
    }
  }

  void SendablePacket::writeDouble ( double d_ )
  {
    if ( _buf )
    {
      _buf->writeDouble ( d_ );
    }
  }

  void SendablePacket::writeString ( const std::string & s_ )
  {
    if ( _buf )
    {
      _buf->writeString ( s_ );
    }
  }

  void SendablePacket::writePacket ( )
  {
    std::size_t before = _buf->getWritePos ( );

    // char = opcode
    // short Packet size
    std::size_t contentStart = before + sizeof ( char ) + sizeof ( short );

    // Write content
    _buf->setWritePos ( contentStart );
    writeImpl ( );
    std::size_t after = _buf->getWritePos ( );

    // Calculate Packet size
    short PacketSize = (short) after - contentStart;

    // Set cursor before Packet content and write opcode and size
    _buf->setWritePos ( before );
    _buf->writeChar ( getOpcode ( ) );
    _buf->writeShort ( PacketSize );

    // Restore write pos
    _buf->setWritePos ( after );
  }

  char ReceivablePacket::readChar ( )
  {
    if ( _buf )
    {
      return _buf->readChar ( );
    }

    return 0;
  }

  short ReceivablePacket::readShort ( )
  {
    if ( _buf )
    {
      return _buf->readShort ( );
    }

    return 0;
  }

  int ReceivablePacket::readInt ( )
  {
    if ( _buf )
    {
      return _buf->readInt ( );
    }

    return 0;
  }

  long ReceivablePacket::readLong ( )
  {
    if ( _buf )
    {
      return _buf->readLong ( );
    }

    return 0;
  }

  float ReceivablePacket::readFloat ( )
  {
    if ( _buf )
    {
      return _buf->readFloat ( );
    }

    return 0.0f;
  }

  double ReceivablePacket::readDouble ( )
  {
    if ( _buf )
    {
      return _buf->readDouble ( );
    }

    return 0.0;
  }

  std::string ReceivablePacket::readString ( )
  {
    if ( _buf )
    {
      return _buf->readString ( );
    }

    return "";
  }

  void ReceivablePacket::run ( )
  {
    executePacketAction ( );
  }
}