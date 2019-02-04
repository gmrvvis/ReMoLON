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
      short packetSize = buffer_.readShort ( );

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
        ReceivablePacketPtr pPtr = factory->createPacket ( );
        ReceivablePacket * p = pPtr.get ( );

        p->setBuffer ( &buffer_ );
        p->setConnection ( &con_ );

        // Read raw data into packet class fields ( programmer-defined )
        p->readImpl ( );

        return pPtr;
      }
      else
      {

        std::string msg = "unknown packet with opcode " + std::to_string ( packetOpcode ) + " from " + con_.address( ).toString( );
        throw std::runtime_error ( msg.c_str ( ) );
        return nullptr;
      }
    }
  }
}