#include "ReMoLON_Util/Client.h"

#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/Net/KeyFileHandler.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/ConsoleCertificateHandler.h>
#include <Poco/Net/SSLManager.h>

namespace remolonUtil
{
  Client::Client ( const std::string & address_, std::uint16_t port_ )
   : _active ( false )
   , _connectAddress ( address_ )
   , _connectPort ( port_ )
  {
  }

  Client::~Client ( )
  {
    close ( );
  }

  void Client::connect ( )
  {
    _active = true;

    initializeConnection ( );

    _recvThread = std::thread ( &Client::recvLoop, this );
    _sendThread = std::thread ( &Client::sendLoop, this );
    _executorThread = std::thread ( &Client::execLoop, this );
  }

  void Client::sendPacket ( SendablePacketPtr & sendable )
  {
    std::unique_lock < std::mutex > lock ( _sendMtx );
    _connection.get ( )->sendPacket ( sendable );
    _sendMonitor.notify_all ( );
  }

  void Client::recvLoop ( )
  {
    Connection * con = _connection.get ( );
    Poco::Net::StreamSocket * socket = con->getSocket ( );

    while ( _active )
    {
      _recvBuffer.reset ( );
      
      int recv = socket->receiveBytes ( _recvBuffer.getData ( ), _recvBuffer.getSize ( ) );

      if ( recv > 0)
      {
        _recvBuffer.setWritePos ( recv );

        ReceivablePacketPtr packetPtr = _packetHandler.handlePacket ( _recvBuffer, *con );

        std::unique_lock < std::mutex > execLock ( _execMtx );
        _execQueue.push ( std::move ( packetPtr ) );
        execLock.unlock ( );
        _execMonitor.notify_all ( );
      }
      else 
      {
        // If connection crashes, receiveBytes will resume with 0 bytes,
        // So recevLoop will be the entry point for handling a crash
        break;
      }
    }

    // Out-of-loop while _active flag is true == connection crash
    if ( _active )
    {
      handleCrash ( );
    }
  }

  void Client::sendLoop ( )
  {
    SendablePacketPtr nextPacket;
    Connection * con = _connection.get ( );
    Poco::Net::StreamSocket * socket = con->getSocket ( );

    while ( _active )
    {
      // Wait until we get notified to send a packet
      std::unique_lock < std::mutex > lock ( _sendMtx );
      while ( ( nextPacket = con->pollSendablePacket ( ) ) == nullptr && _active )
      {
        _sendMonitor.wait ( lock );
      }

      if ( nextPacket != nullptr  && nextPacket.get ( ) != nullptr )
      {
        _sendBuffer.reset ( );
        SendablePacket * sp = nextPacket.get ( );
        sp->setBuffer ( &_sendBuffer );
        sp->writePacket ( );

        socket->sendBytes ( _sendBuffer.getData ( ), _sendBuffer.getUsedSize ( ) );
      }
    }
  }

  void Client::execLoop ( )
  {
    while ( _active )
    {
      std::unique_lock < std::mutex > execLock ( _execMtx );
      while ( _execQueue.empty ( ) && _active )
      {
        _execMonitor.wait ( execLock );
      }

      if ( !_execQueue.empty ( ) )
      {
        ReceivablePacketPtr next = std::move ( _execQueue.front ( ) );
        _execQueue.pop ( );

        if ( next != nullptr)
        {
          next.get ( )->run ( );
        }
      }

      execLock.unlock ( );
    }
  }

  void Client::close ( )
  {
    _active = false;

    _connection.get ( )->endConnection ( );
    _sendMonitor.notify_all ( );
    _execMonitor.notify_all ( );

    if ( _recvThread.joinable ( ) )
    {
      _recvThread.join ( );
    }

    if ( _sendThread.joinable ( ) )
    {
      _sendThread.join ( );
    }

    if ( _executorThread.joinable ( ) )
    {
      _executorThread.join ( );
    }

    Poco::Net::uninitializeSSL ( );
  }

  void Client::handleCrash ( )
  {
    _active = false;

    _connection.get ( )->endConnection ( );
    _sendMonitor.notify_all ( );
    _execMonitor.notify_all ( );
    
    if ( _sendThread.joinable ( ) )
    {
      _sendThread.join ( );
    }
    
    if ( _executorThread.joinable ( ) )
    {
      _executorThread.join ( );
    }
    
    Poco::Net::uninitializeSSL ( );
  }

  // ==============================================================================================

  RawClient::RawClient ( const std::string & address_, std::uint16_t port_ )
   : Client ( address_, port_ )
  {
  }

  void RawClient::initializeConnection ( )
  {
    _connection = std::make_unique < RawConnection > ( _connectAddress, _connectPort );
  }

  // ==============================================================================================

  SecureClient::SecureClient ( const std::string & address_, std::uint16_t port_,
                               const std::string & keyFile_,
                               const std::string & certFile_,
                               const std::string & caFile_)
   : Client ( address_, port_ )
   , _keyFile ( keyFile_ )
   , _certFile ( certFile_ )
   , _caFile ( caFile_ )
  {
  }

  void SecureClient::initializeConnection ( )
  {
    _connection = std::make_unique < SecureConnection > ( _connectAddress, 
                                                          _connectPort,
                                                          _keyFile,
                                                          _certFile,
                                                          _caFile );
  }
}