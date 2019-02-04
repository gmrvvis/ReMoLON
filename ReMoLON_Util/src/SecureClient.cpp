#include "ReMoLON_Util/SecureClient.h"

#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/Net/KeyFileHandler.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/ConsoleCertificateHandler.h>
#include <Poco/Net/SSLManager.h>

namespace remolonUtil
{
  SecureClient::SecureClient ( const TClientConfig & clientCfg_ )
   : _active ( false )
  {
    _connectAddress = clientCfg_._serverAddress;
    _connectPort = clientCfg_._serverPort;

    _keyFile = clientCfg_._keyFilePath;
    _certFile = clientCfg_._certFilePath;
    _caFile = clientCfg_._caFilePath;
  }

  SecureClient::~SecureClient ( )
  {
    close ( );
  }

  void SecureClient::connect ( )
  {
    _active = true;

    Poco::Net::initializeSSL();

    initializeSocket ( );

    _recvThread = std::thread ( &SecureClient::recvLoop, this );
    _sendThread = std::thread ( &SecureClient::sendLoop, this );
    _executorThread = std::thread ( &SecureClient::execLoop, this );
  }

  void SecureClient::initializeSocket ( )
  {
    Poco::SharedPtr<Poco::Net::PrivateKeyPassphraseHandler> keyHandler = new Poco::Net::KeyFileHandler(false);
    Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> invalidCertHandler = new Poco::Net::ConsoleCertificateHandler(true);

    Poco::Net::Context::Ptr context = new Poco::Net::Context
    (
      Poco::Net::Context::CLIENT_USE, 
      _keyFile,
      _certFile,
      _caFile, 
      Poco::Net::Context::VERIFY_NONE, 
      9, 
      false, 
      "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
    );

    context->enableSessionCache(true);

    Poco::Net::SSLManager::instance().initializeClient(keyHandler, invalidCertHandler, context);

    _connection = std::make_unique < Connection > ( _connectAddress, 
                                                    _connectPort,
                                                    context );
  }

  void SecureClient::sendPacket ( SendablePacketPtr & sendable )
  {
    std::unique_lock < std::mutex > lock ( _sendMtx );
    _connection.get ( )->sendPacket ( sendable );
    _sendMonitor.notify_all ( );
  }

  void SecureClient::recvLoop ( )
  {
    Connection * con = _connection.get ( );

    while ( _active )
    {
      _recvBuffer.reset ( );
      
      int recv = con->receiveBytes ( _recvBuffer.getData ( ), _recvBuffer.getSize ( ) );

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

  void SecureClient::sendLoop ( )
  {
    SendablePacketPtr nextPacket;
    Connection * con = _connection.get ( );

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

        con->sendBytes ( _sendBuffer.getData ( ), _sendBuffer.getUsedSize ( ) );
      }
    }
  }

  void SecureClient::execLoop ( )
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

  void SecureClient::close ( )
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

  void SecureClient::handleCrash ( )
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
}