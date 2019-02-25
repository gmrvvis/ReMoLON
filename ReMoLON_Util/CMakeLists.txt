#-------------------------------------------------------------------------------
# ReMoLON Utility library
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
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src)

set( REMOLON_UTIL_SOURCES   src/ByteBuffer.cpp
                            src/Connection.cpp
                            src/Packet.cpp
                            src/PacketHandler.cpp
                            src/SelectorThread.cpp
                            src/Server.cpp
                            src/Config.cpp
                            src/Client.cpp
)

set( REMOLON_UTIL_PUBLIC_HEADERS  include/ReMoLON_Util/ByteBuffer.h
                                  include/ReMoLON_Util/Connection.h
                                  include/ReMoLON_Util/Packet.h
                                  include/ReMoLON_Util/PacketHandler.h
                                  include/ReMoLON_Util/Runnable.h
                                  include/ReMoLON_Util/SelectorThread.h
                                  include/ReMoLON_Util/ThreadPool.h
                                  include/ReMoLON_Util/Server.h
                                  include/ReMoLON_Util/Config.h
                                  include/ReMoLON_Util/Client.h
)

set(REMOLON_UTIL_NAMESPACE remolon_util)
set(REMOLON_UTIL_INCLUDE_NAMES ReMoLON_Util )

set(REMOLON_UTIL_LINK_LIBRARIES ${Poco_Util_LIBRARY}
                                ${Poco_NetSSL_LIBRARY}
                                ${Poco_Crypto_LIBRARY}
                                ${Poco_Net_LIBRARY}
                                ${Poco_Foundation_LIBRARY}
)

common_library(remolon_util)
