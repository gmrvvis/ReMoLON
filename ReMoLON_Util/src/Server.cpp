#include "ReMoLON_Util/Server.h"

namespace remolonUtil
{
  Server::Server ( const TServerConfig & serverConfig_ )
   : SecureServer ( serverConfig_ )
  {
  }

  void Server::initializeSocket ( )
  {
    Poco::Net::SocketAddress sa ( _bindIPAddress, _bindPort );

    Poco::Net::initializeSSL ( );

    Poco::Net::Context::Ptr context = new Poco::Net::Context
    (
      Poco::Net::Context::SERVER_USE, 
      "",
      "",
      "", 
      Poco::Net::Context::VERIFY_NONE, 
      9, 
      false, 
      "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
    );

    context->enableSessionCache ( true );

    _serverSocket = std::make_unique < Poco::Net::SecureServerSocket >( sa, 64, context );
  }
}
