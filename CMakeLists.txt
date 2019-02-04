#-------------------------------------------------------------------------------
# desktop streamer frontend server
#
# Copyright (c) 2018 Rey Juan Carlos University, Spain,
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

cmake_minimum_required(VERSION 3.6)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

find_package(Poco REQUIRED Util XML JSON NetSSL Crypto Net Foundation)

#if (${Poco_FOUND})
#	include_directories(rr-frontend-server ${Poco_INCLUDE_DIRS})
#	target_link_libraries(rr-frontend-server ${Poco_Util_LIBRARY})
#	target_link_libraries(rr-frontend-server ${Poco_XML_LIBRARY})
#	target_link_libraries(rr-frontend-server ${Poco_JSON_LIBRARY})
#	target_link_libraries(rr-frontend-server ${Poco_NetSSL_LIBRARY})
#	target_link_libraries(rr-frontend-server ${Poco_Crypto_LIBRARY})
#	target_link_libraries(rr-frontend-server ${Poco_Net_LIBRARY})
#	target_link_libraries(rr-frontend-server ${Poco_Foundation_LIBRARY})
#else()
#	message(FATAL_ERROR "Some or all Poco components not found, STOPPING.")
#endif (${Poco_FOUND})

find_package(PAM REQUIRED)

#if (${PAM_FOUND})
#  target_include_directories(rr-frontend-server PUBLIC ${PAM_INCLUDE_DIRS})
#  target_link_libraries(rr-frontend-server ${PAM_LIBRARIES})
#else()
#  message(FATAL_ERROR "PAM not found, STOPPING.")
#endif (${PAM_FOUND})

find_package (Threads REQUIRED)

#target_link_libraries (rr-frontend-server ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(ReMoLON_Util)
add_subdirectory(ReMoLON)
add_subdirectory(ReMoLON_FrontEnd)
