#-------------------------------------------------------------------------------
# ReMoLON
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
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/frontendclientpackets )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/frontendserverpackets )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/remotooclientpackets )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/remotooserverpackets )

include_directories( ${PROJECT_SOURCE_DIR}
                     ${PROJECT_SOURCE_DIR}/ReMoLON_Util/include/
                     ${PROJECT_BINARY_DIR} )
                     
set( REMOLON_SOURCES  src/main.cpp
                      src/Node.cpp
                      src/SessionManager.cpp
                      src/StreamingSession.cpp
                      src/frontendclientpackets/CloseSessionResult.cpp
                      src/frontendclientpackets/NodeInfo.cpp
                      src/frontendclientpackets/StartStreamingSessionResult.cpp
                      src/frontendserverpackets/RequestNodeInfo.cpp
                      src/frontendserverpackets/RequestStartStreamingSession.cpp
                      src/frontendserverpackets/RequestCloseUserSession.cpp
                      src/remotooclientpackets/AllowAddressAnswer.cpp
                      src/remotooclientpackets/SessionInfo.cpp
                      src/remotooserverpackets/RequestSessionInfo.cpp
                      src/remotooserverpackets/RequestAllowAddress.cpp
                      src/remotooserverpackets/RequestFinishSession.cpp
)

set( REMOLON_PUBLIC_HEADERS include/Node.h
                            include/SessionManager.h
                            include/SessionManager.h
                            include/frontendclientpackets/CloseSessionResult.h
                            include/frontendclientpackets/NodeInfo.h
                            include/frontendclientpackets/StartStreamingSessionResult.h
                            include/frontendserverpackets/RequestNodeInfo.h
                            include/frontendserverpackets/RequestStartStreamingSession.h
                            include/frontendserverpackets/RequestCloseUserSession.h
                            include/remotooclientpackets/AllowAddressAnswer.h
                            include/remotooclientpackets/SessionInfo.h
                            include/remotooserverpackets/RequestSessionInfo.h
                            include/remotooserverpackets/RequestAllowAddress.h
                            include/remotooserverpackets/RequestFinishSession.h
)

set(REMOLON_NAMESPACE remolon)
set(REMOLON_INCLUDE_NAMES ReMoLON )

set(REMOLON_LINK_LIBRARIES  remolon_util
                            Threads::Threads)

common_application( remolon )
