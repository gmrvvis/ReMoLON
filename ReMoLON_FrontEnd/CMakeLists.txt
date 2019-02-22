#-------------------------------------------------------------------------------
# ReMoLON FrontEnd Server
#
# Copyright (c) 2019    Rey Juan Carlos University, Spain,
#                           Modeling, Virtual Reality, and Visualization Group.
#                       Universidad Politécnica de Madrid, Spain
#                           Center for Computational Simulation
#-------------------------------------------------------------------------------
#                                 License
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#-------------------------------------------------------------------------------

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/serverpackets)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/clientpackets)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/httpactions)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories( ${PROJECT_SOURCE_DIR}
                     ${PROJECT_SOURCE_DIR}/ReMoLON_Util/include/
                     ${PROJECT_BINARY_DIR} )

set( REMOLON_FRONTEND_SOURCES   src/main.cpp
                                src/RequestManager.cpp
                                src/SessionManagerServer.cpp
                                src/UserManager.cpp
                                src/WebServer.cpp
                                src/WebServerRequestHandler.cpp
                                src/XmlBuilder.cpp
                                src/SessionTracker.cpp
                                src/User.cpp
                                src/VisualizationNode.cpp
                                src/VisualizationNodeManager.cpp
                                src/httpactions/DefaultActionHandler.cpp
                                src/httpactions/CreateSessionActionHandler.cpp
                                src/httpactions/SessionListActionHandler.cpp
                                src/httpactions/LogInActionHandler.cpp
                                src/httpactions/DestroySessionActionHandler.cpp
                                src/clientpackets/CloseSessionResult.cpp
                                src/clientpackets/NodeInfo.cpp
                                src/clientpackets/StartStreamingSessionResult.cpp
                                src/serverpackets/RequestCloseUserSession.cpp
                                src/serverpackets/RequestNodeInfo.cpp
                                src/serverpackets/RequestStartStreamingSession.cpp
)

set( REMOLON_FRONTEND_PUBLIC_HEADERS  include/AbstractActionHandler.h
                                      include/RequestManager.h
                                      include/SessionManagerServer.h
                                      include/UserManager.h
                                      include/WebServer.h
                                      include/WebServerRequestHandler.h
                                      include/XmlBuilder.h
                                      include/SessionTracker.h
                                      include/User.h
                                      include/VisualizationNode.h
                                      include/VisualizationNodeManager.h
                                      include/httpactions/DefaultActionHandler.h
                                      include/httpactions/CreateSessionActionHandler.h
                                      include/httpactions/SessionListActionHandler.h
                                      include/httpactions/LogInActionHandler.h
                                      include/httpactions/DestroySessionActionHandler.h
                                      include/clientpackets/CloseSessionResult.h
                                      include/clientpackets/NodeInfo.h
                                      include/clientpackets/StartStreamingSessionResult.h
                                      include/serverpackets/RequestCloseUserSession.h
                                      include/serverpackets/RequestNodeInfo.h
                                      include/serverpackets/RequestStartStreamingSession.h
)

set(REMOLON_FRONTEND_NAMESPACE ReMoLONFrontEnd)
set(REMOLON_FRONTEND_INCLUDE_NAMES ReMoLONFrontEnd )

set(REMOLON_FRONTEND_LINK_LIBRARIES remolon_util
                                    PocoUtil
                                    PocoNetSSL
                                    PocoCrypto
                                    PocoNet
                                    PocoFoundation
                                    ${PAM_LIBRARY}
                                    )

common_application( remolon_frontend )
