#-------------------------------------------------------------------------------
# ReMoLON Utility library
#
# Copyright (c) 2019 Rey Juan Carlos University, Spain,
# Modeling, Virtual Reality, and Visualization Group.
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

list(APPEND REMOLON_UTIL_SOURCES src/ByteBuffer.cpp)
list(APPEND REMOLON_UTIL_SOURCES src/Connection.cpp)
list(APPEND REMOLON_UTIL_SOURCES src/Packet.cpp)
list(APPEND REMOLON_UTIL_SOURCES src/PacketHandler.cpp)
list(APPEND REMOLON_UTIL_SOURCES src/SelectorThread.cpp)
list(APPEND REMOLON_UTIL_SOURCES src/Server.cpp)
list(APPEND REMOLON_UTIL_SOURCES src/Config.cpp)
list(APPEND REMOLON_UTIL_SOURCES src/Client.cpp)

list(APPEND REMOLON_UTIL_HEADERS include/ReMoLON_Util/ByteBuffer.h)
list(APPEND REMOLON_UTIL_HEADERS include/ReMoLON_Util/Connection.h)
list(APPEND REMOLON_UTIL_HEADERS include/ReMoLON_Util/Packet.h)
list(APPEND REMOLON_UTIL_HEADERS include/ReMoLON_Util/PacketHandler.h)
list(APPEND REMOLON_UTIL_HEADERS include/ReMoLON_Util/Runnable.h)
list(APPEND REMOLON_UTIL_HEADERS include/ReMoLON_Util/SelectorThread.h)
list(APPEND REMOLON_UTIL_HEADERS include/ReMoLON_Util/ThreadPool.h)
list(APPEND REMOLON_UTIL_HEADERS include/ReMoLON_Util/Server.h)
list(APPEND REMOLON_UTIL_HEADERS include/ReMoLON_Util/Config.h)
list(APPEND REMOLON_UTIL_HEADERS include/ReMoLON_Util/Client.h)

add_library(remolonutil SHARED
	${REMOLON_UTIL_HEADERS}
  ${REMOLON_UTIL_SOURCES}
)

target_include_directories(remolonutil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (${Poco_FOUND})
  list(APPEND REMOLON_UTIL_INCLUDES ${Poco_INCLUDE_DIRS})
	#list(APPEND REMOLON_UTIL_LIBS ${Poco_Util_LIBRARY})
	list(APPEND REMOLON_UTIL_LIBS ${Poco_NetSSL_LIBRARY})
	list(APPEND REMOLON_UTIL_LIBS ${Poco_Crypto_LIBRARY})
	list(APPEND REMOLON_UTIL_LIBS ${Poco_Net_LIBRARY})
	list(APPEND REMOLON_UTIL_LIBS ${Poco_Foundation_LIBRARY})
else()
	message(FATAL_ERROR "Some or all Poco components not found, STOPPING.")
endif (${Poco_FOUND})


list(APPEND REMOLON_UTIL_LIBS ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(remolonutil PUBLIC ${REMOLON_UTIL_INCLUDES})
target_link_libraries(remolonutil ${REMOLON_UTIL_LIBS})

configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/remolonutil-config.cmake.in
  "${PROJECT_BINARY_DIR}/remolonutil-config.cmake" @ONLY)
export(TARGETS remolonutil
  FILE "${PROJECT_BINARY_DIR}/remolonutil-targets.cmake")
