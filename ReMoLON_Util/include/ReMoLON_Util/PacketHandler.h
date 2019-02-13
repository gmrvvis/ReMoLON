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
 
#ifndef __REMOLON_UTIL_PACKETHANDLER_H__
#define __REMOLON_UTIL_PACKETHANDLER_H__

#include <stdexcept>
#include <vector>

#include "ReMoLON_Util/Packet.h"

namespace remolonUtil
{
  class AbstractPacketFactory
  {
    public:
      AbstractPacketFactory ( )
      {

      }

      virtual ReceivablePacketPtr createPacket ( ) = 0;
  };

  typedef std::unique_ptr < AbstractPacketFactory > AbstractPacketFactoryPtr;

  template<class T>
  class PacketFactory : public AbstractPacketFactory
  {
    public:
      PacketFactory ( )
      {

      }

      ReceivablePacketPtr createPacket ( )
      {
        ReceivablePacketPtr ptr =  std::make_unique < T > ( );
        return ptr;
      }
  };

  class PacketHandler
  {
    public:
      PacketHandler ( );

      ~PacketHandler ( );

      template < class T >
      void registerPacketFactory ( )
      {
        if ( std::is_base_of < ReceivablePacket, T >::value)
        {
          AbstractPacketFactoryPtr factory = std::make_unique < PacketFactory< T > > ( );
          ReceivablePacketPtr tempPacket = factory.get ( )->createPacket ( );

          int opcode = tempPacket.get ( )->getOpcode ( );
          if ( opcode >= _packetFactories.size ( ) )
          {
            throw std::runtime_error ( "packetHandler: Attempted to register a packet with a higher opcode than supoorted");
          }

          _packetFactories [ opcode ] = std::move ( factory );
        }
        else
        {
          throw std::runtime_error ( "packetHandler: Attempted to create a packet factory of a non-derived packet class" );
        }
      }

      ReceivablePacketPtr handlePacket ( ByteBuffer & buffer_, Connection & connection );

    private:
      std::vector < AbstractPacketFactoryPtr > _packetFactories;

      ReceivablePacketPtr pPtr;
  };
}

#endif