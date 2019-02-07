#include "StreamingSession.h"

#include <pwd.h>
#include <sys/wait.h>
#include <stdexcept>

#include "Node.h"

#include "remotooserverpackets/RequestFinishSession.h"

#include <iostream>

namespace remolon
{
  StreamingSession::StreamingSession ( )
  {

  }

  void StreamingSession::setSessionName ( const std::string & sessionName_ )
  {
    _sessionName = sessionName_;
  }

  void StreamingSession::setSessionOwner ( const std::string & sessionOwner_ )
  {
    _sessionOwner = sessionOwner_;
  }

  void StreamingSession::setSessionOwnerAddress ( const std::string & ownerAddress_ )
  {
    _sessionOwnerAddress = ownerAddress_;
  }

  void StreamingSession::setSessionPorts ( uint16_t port_, uint16_t sockPort_, uint16_t rtcPort_ )
  {
    _sessionPort = port_;
    _sessionSockPort = sockPort_;
    _sessionRTCPort = rtcPort_;
  }

  void StreamingSession::setConnection ( remolonUtil::Connection * con_ )
  {
    _connection = con_;
  }

  void StreamingSession::addAllowedUser ( const std::string & userName_,
                                           const std::string & userAddress_ )
  {
    std::unique_lock < std::mutex > lock ( _mtx );
    _allowedUsers [ userName_ ] = userAddress_;
    // TODO: Send packet to streaming process thought its _connection object
  }

  const std::string & StreamingSession::getSessionName ( )
  {
    return _sessionName;
  }

  const std::string & StreamingSession::getSessionOwner ( )
  {
    return _sessionOwner;
  }

  const std::string & StreamingSession::getSessionOwnerAddress ( )
  {
    return _sessionOwnerAddress;
  }

  uint16_t StreamingSession::getSessionPort ( )
  {
    return _sessionPort;
  }

  uint16_t StreamingSession::getSessionSocketPort ( )
  {
    return _sessionSockPort;
  }

  uint16_t StreamingSession::getSessionRTCPort ( )
  {
    return _sessionRTCPort;
  }

  bool StreamingSession::tryLaunchSession ( )
  {
    bool result = true;
    try
    {
      _streamingProcess = std::thread( [ this ]
      {
          int child = fork ( );
          if ( child == 0 )
          {
            const std::string & user = this->getSessionOwner ( );
            struct passwd * userInfo = getpwnam( user.c_str ( ) );
            char buf [ 0xffff ];
            sprintf ( buf, "HOME=%s", userInfo->pw_dir );
            char * env [ ] = { buf, NULL };

            const std::string & sessionName = this->getSessionName ( );
            const std::string & ownerAddress = this->getSessionOwnerAddress ( );
            std::string webPort = std::to_string ( this->getSessionPort ( ) );
            std::string sockPort = std::to_string ( this->getSessionSocketPort ( ) );
            std::string rtcPort = std::to_string( this->getSessionRTCPort ( ) );

            std::string xinitrcBin = Node::getInstance ( ).getRemolonXinitrcBinaryDir ( );
            xinitrcBin += xinitrcBin [ xinitrcBin.length ( ) - 1 ] == '/'? "" : "/";
            xinitrcBin += "remolon_xinit_writter";

            const std::string & remolonBin = Node::getInstance ( ).getRemotooBinaryDir ( );

            // Re-write .xinitrc file
            execle( "/usr/bin/sudo", 
                    "sudo", 
                    "-u", 
                    user.c_str ( ), 
                    xinitrcBin.c_str ( ), 
                    remolonBin.c_str ( ), 
                    sessionName.c_str ( ),
                    user.c_str ( ),
                    ownerAddress.c_str ( ),
                    webPort.c_str ( ),
                    sockPort.c_str ( ),
                    rtcPort.c_str ( ),
                    (char*)0, 
                    env );

            // Launch x session
            //execle( "/usr/bin/sudo", "sudo", "-u", user.c_str ( ), "xinit", "--", ":4", (char*)0, env );
          }
          else if ( child > 0 )
          {
            int status;
            waitpid ( child, &status, 0 );
            if ( status != 0 )
            {
              // Handle error on session closing (or even startup)
            }
          }
      });
    }
    catch( std::exception & e )
    {
      result = false;
    }

    return result;
  }

  void StreamingSession::finishSession ( )
  {
    remolonUtil::SendablePacketPtr 
    finishPacket = std::make_unique< remotooserverpackets::RequestFinishSession > ( );
    _connection->sendPacket ( finishPacket );
  }
}
