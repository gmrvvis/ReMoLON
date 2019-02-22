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

cmake_minimum_required ( VERSION 3.2 )
project ( ReMoLON VERSION 0.0.1 )
set( ReMoLON_VERSION_ABI 1 )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif( NOT CMAKE_BUILD_TYPE )

if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
    message( FATAL "no in source building allowed." )
endif( )

if( NOT EXISTS ${CMAKE_SOURCE_DIR}/CMake/common/Common.cmake )
    message( FATAL_ERROR "CMake/common missing, run: git submodule update --init")
endif( )

list( APPEND CMAKE_MODULE_PATH
 ${PROJECT_SOURCE_DIR}/CMake
 ${CMAKE_SOURCE_DIR}/CMake/common )

include( GitExternal )

set( REMOLON_DESCRIPTION "ReMoLON: Remote Movie Linker Session." )
set( REMOLON_LICENSE LGPL )
set( COMMON_PROJECT_DOMAIN es.gmrv )

include( Common )

common_find_package( Poco REQUIRED COMPONENTS Util XML JSON NetSSL Crypto Net Foundation )
common_find_package( PAM REQUIRED SYSTEM )
common_find_package( Threads REQUIRED )

common_find_package_post( )

add_subdirectory(ReMoLON_Util)
add_subdirectory(ReMoLON)
add_subdirectory(ReMoLON_FrontEnd)
add_subdirectory(ReMoLON_xinitrc_writter)

file(COPY ${PROJECT_SOURCE_DIR}/remolonClientConfig.cfg DESTINATION "${CMAKE_BINARY_DIR}/bin")
file(COPY ${PROJECT_SOURCE_DIR}/frontendServerConfig.cfg DESTINATION "${CMAKE_BINARY_DIR}/bin")
file(GLOB CLIENT_CONTENT "client/*")
file(COPY ${CLIENT_CONTENT} DESTINATION "${CMAKE_BINARY_DIR}/bin/FEClient")

set( DOCS README.md LICENSE.txt )
install( FILES ${DOCS} DESTINATION share/ReMoLON COMPONENT dev )

set( DOXYGEN_MAINPAGE_MD README.md )
set( DOXYGEN_EXTRA_INPUT ${PROJECT_SOURCE_DIR}/README.md )
include( DoxygenRule )

include( CPackConfig )
include( CTest )
