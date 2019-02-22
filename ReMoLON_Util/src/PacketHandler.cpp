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
 
#include "ReMoLON_Util/PacketHandler.h"

#include "ReMoLON_Util/Connection.h"

namespace remolonUtil
{
  PacketHandler::PacketHandler ( )
  {
    _packetFactories.resize ( 256 );

    //registerPacketFactory< echoPacket > ( );
  }

  PacketHandler::~PacketHandler ( )
  {

  }

  ReceivablePacketPtr PacketHandler::handlePacket ( ByteBuffer & buffer_, Connection & con_ )
  {
    while ( buffer_.getReadPos ( ) < buffer_.getWritePos ( ) )
    {
      std::size_t packetBeginsAt = buffer_.getReadPos ( );

      char packetOpcode = buffer_.readChar ( );
      unsigned short packetSize = buffer_.readShort ( );

      // Cocrrupted size ( packet larger than buffer )
      if ( packetSize > buffer_.getSize ( ) && packetBeginsAt == 0)
      {
        throw std::runtime_error ( "packet size is bigger than buffer " );
        break;
      }

      // We didnt receive the whole packet, compacket buffer
      // and let it ready for next reading to complete the packet
      if ( packetSize > buffer_.getRemainingReadSize ( ) )
      {
        // Put the read cursor at the begining so compact will preserve opcode and size
        buffer_.setReadPos ( packetBeginsAt );
        buffer_.compact ( );
        return nullptr;
      }

      AbstractPacketFactory * factory = _packetFactories [ packetOpcode ].get ( );
      if(factory)
      {
        //### Shadowing
        ReceivablePacketPtr pPtr2 = factory->createPacket ( );
        ReceivablePacket * p = pPtr2.get ( );

        p->setBuffer ( &buffer_ );
        p->setConnection ( &con_ );

        // Read raw data into packet class fields ( programmer-defined )
        p->readImpl ( );

        return pPtr2;
      }
      else
      {
        std::string msg = "unknown packet with opcode " + std::to_string ( packetOpcode ) + " from " + con_.clientAddress ( );
        throw std::runtime_error ( msg.c_str ( ) );
        return nullptr;
      }
    }
    //###
    return nullptr;
  }
}
