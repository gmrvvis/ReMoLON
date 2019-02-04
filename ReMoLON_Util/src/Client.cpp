#include "ReMoLON_Util/Client.h"

namespace remolonUtil
{
  Client::Client ( const TClientConfig & config_ )
   : SecureClient ( config_ )
  {
  }

  void Client::initializeSocket ( )
  {
    _active = true;

    Poco::Net::initializeSSL();

    Poco::Net::Context::Ptr context = new Poco::Net::Context
    (
      Poco::Net::Context::CLIENT_USE, 
      "",
      "",
      "", 
      Poco::Net::Context::VERIFY_NONE, 
      9, 
      false, 
      "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
    );

    context->enableSessionCache(true);

    _connection = std::make_unique < Connection > ( _connectAddress, 
                                                    _connectPort,
                                                    context );
  }
}