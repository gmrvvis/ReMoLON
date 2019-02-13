ReMoLON
=====================================================

## Introduction

ReMoLON is a software system designed and implemented to work with ReMoToo.
It's goal is to manage any amount of visualization nodes, in which it will
spawn the ReMoToo system based on user requests, providing a front-end interface
from where those users will be able to authenticate and manage streaming sessions.

## Software structure

ReMoLON is splitted in 2 main components:
* ReMoLON Frontend: Is the authentication daemon, which will serve a login page,
  as well as a session management page to the user once authenticated. It is also
  the manager of all visualization nodes that will be available to the users, acting
  as a server, keeping track of streaming activity and balancing the load of streaming 
  session requests.
* ReMoLON: Is the manager of a single visualization node. It's goal is to launch and
  control the streaming sessions which will be executed on that node. The requested 
  actions will come from ReMoLON Frontend, to which ReMoLON acts as a client.

Besides the 2 main components, 2 aditional utilities are used with the system:
* ReMoLON Util: Is a utility library which encapsulates and eases the use of
  sockets under a custom TCP/IP protocol
* ReMoLON xinitrc writter: A small program which is used to launch the streaming
  session under the requesting user environment, and with the appropiate configuration
  stablished by him.

## Dependencies
* Required dependencies:
    * Poco (Components: Util XML JSON NetSSL Crypto Net Foundation)
    * PAM (Linux Authentication Modules)
    * Threads

## Building

ReMoLON has been successfully built and used on Ubuntu 18.04 LTS. 
The following steps should be enough to build it. Nevertheless, to use this software,
a set of configurations steps are required, and ReMoToo should be present on the
system in order to access full functionality. As stated above, all the dependencies
are required to build the software.

## Description

```bash
git clone -b InitialRelease https://gitlab.gmrv.es/n.roman/remolon.git
mkdir remolon/build && cd remolon/build
cmake ..
make
```

## Configuration

There are 2 configuration files which must be edited, if needed, to make ReMoLON
work properly:
* frontendServerConfig.cfg: Located in the ReMolON Frontend build directory after
  compilation. In this file the address and port to which the ReMoLON instances 
  management server will be binded (and, thus, to which address and port the ReMoLON
  instances must be configured to connect to). It also includes, as well, the settings
  to specify the path to the Key, Certificate, and Chan of Authority certificate files
  if ReMoLON uses SSL for its communications.
* remolonClientConfig.cfg: Located in the ReMoLON build directory. Contains settings to:
    * Configure the address and port where ReMoLON Frontend will be listening for
      connections (the same as specified in frontendServerConfig.cfg)
    * Configure the Key, Certificate, and Chain of Authority certificate if ReMoLON and
      ReMoLON Frontend are build to use SSL
    * The public address which should be accessible by the end users, used to bind the
      streaming web servers of each session.
    * The ports available to ReMoLON on which to launch streaming sessions. The are 3 port
      lists, one for webservers, one for websocket-based streaming, and one for webrtc-based
      streaming. They all must have the same amount of items, and the ports should be unique
      across the 3 lists.
