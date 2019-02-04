list(APPEND REMOLON_SOURCES src/main.cpp)
list(APPEND REMOLON_SOURCES src/Node.cpp)
list(APPEND REMOLON_SOURCES src/SessionManager.cpp)
list(APPEND REMOLON_SOURCES src/StreamingSession.cpp)
list(APPEND REMOLON_SOURCES src/frontendclientpackets/CloseSessionResult.cpp)
list(APPEND REMOLON_SOURCES src/frontendclientpackets/NodeInfo.cpp)
list(APPEND REMOLON_SOURCES src/frontendclientpackets/StartStreamingSessionResult.cpp)
list(APPEND REMOLON_SOURCES src/frontendserverpackets/RequestNodeInfo.cpp)
list(APPEND REMOLON_SOURCES src/frontendserverpackets/RequestStartStreamingSession.cpp)
list(APPEND REMOLON_SOURCES src/frontendserverpackets/RequestCloseUserSession.cpp)
list(APPEND REMOLON_SOURCES src/remotooclientpackets/AllowAddressAnswer.cpp)
list(APPEND REMOLON_SOURCES src/remotooserverpackets/RequestAllowAddress.cpp)
list(APPEND REMOLON_SOURCES src/remotooserverpackets/RequestFinishSession.cpp)

list(APPEND REMOLON_HEADERS include/Node.h)
list(APPEND REMOLON_HEADERS include/SessionManager.h)
list(APPEND REMOLON_HEADERS include/SessionManager.h)
list(APPEND REMOLON_HEADERS include/frontendclientpackets/CloseSessionResult.h)
list(APPEND REMOLON_HEADERS include/frontendclientpackets/NodeInfo.h)
list(APPEND REMOLON_HEADERS include/frontendclientpackets/StartStreamingSessionResult.h)
list(APPEND REMOLON_HEADERS include/frontendserverpackets/RequestNodeInfo.h)
list(APPEND REMOLON_HEADERS include/frontendserverpackets/RequestStartStreamingSession.h)
list(APPEND REMOLON_HEADERS include/frontendserverpackets/RequestCloseUserSession.h)
list(APPEND REMOLON_HEADERS include/remotooclientpackets/AllowAddressAnswer.h)
list(APPEND REMOLON_HEADERS include/remotooserverpackets/RequestAllowAddress.h)
list(APPEND REMOLON_HEADERS include/remotooserverpackets/RequestFinishSession.h)

list(APPEND REMOLON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND REMOLON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/frontendclientpackets)
list(APPEND REMOLON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/frontendserverpackets)
list(APPEND REMOLON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/remotooclientpackets)
list(APPEND REMOLON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/remotooserverpackets)

include_directories(${REMOLON_INCLUDE_DIRS})

add_executable(remolon
	${REMOLON_HEADERS}
  ${REMOLON_SOURCES}
)

target_link_libraries (remolon remolonutil)

# copy configuration file to binary directory
configure_file(${CMAKE_SOURCE_DIR}/remolonClientConfig.cfg ${CMAKE_CURRENT_BINARY_DIR}/remolonClientConfig.cfg COPYONLY)