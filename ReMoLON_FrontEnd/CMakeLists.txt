list(APPEND REMOLON_FRONTEND_SOURCES src/main.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/RequestManager.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/SessionManagerServer.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/UserManager.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/WebServer.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/WebServerRequestHandler.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/XmlBuilder.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/SessionTracker.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/User.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/VisualizationNode.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/VisualizationNodeManager.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/httpactions/DefaultActionHandler.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/httpactions/CreateSessionActionHandler.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/httpactions/SessionListActionHandler.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/httpactions/LogInActionHandler.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/clientpackets/CloseSessionResult.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/clientpackets/NodeInfo.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/clientpackets/StartStreamingSessionResult.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/serverpackets/RequestCloseUserSession.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/serverpackets/RequestNodeInfo.cpp)
list(APPEND REMOLON_FRONTEND_SOURCES src/serverpackets/RequestStartStreamingSession.cpp)

list(APPEND REMOLON_FRONTEND_HEADERS include/AbstractActionHandler.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/RequestManager.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/SessionManagerServer.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/UserManager.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/WebServer.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/WebServerRequestHandler.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/XmlBuilder.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/SessionTracker.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/User.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/VisualizationNode.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/VisualizationNodeManager.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/httpactions/DefaultActionHandler.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/httpactions/CreateSessionActionHandler.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/httpactions/SessionListActionHandler.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/httpactions/LogInActionHandler.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/clientpackets/CloseSessionResult.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/clientpackets/NodeInfo.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/clientpackets/StartStreamingSessionResult.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/serverpackets/RequestCloseUserSession.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/serverpackets/RequestNodeInfo.h)
list(APPEND REMOLON_FRONTEND_HEADERS include/serverpackets/RequestStartStreamingSession.h)

list(APPEND REMOLON_FRONTEND_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND REMOLON_FRONTEND_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/serverpackets)
list(APPEND REMOLON_FRONTEND_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/clientpackets)
list(APPEND REMOLON_FRONTEND_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/httpactions)

include_directories(${REMOLON_FRONTEND_INCLUDE_DIRS})

add_executable(remolon_frontend_server
	${REMOLON_FRONTEND_HEADERS}
  ${REMOLON_FRONTEND_SOURCES}
)

# Poco includes
list(APPEND REMOLON_FRONTEND_INCLUDE_DIRS ${Poco_INCLUDE_DIRS})
# Poco libraries
list(APPEND REMOLON_FRONTEND_LIBRARY_DIRS ${Poco_Util_LIBRARY})
list(APPEND REMOLON_FRONTEND_LIBRARY_DIRS ${Poco_XML_LIBRARY})
list(APPEND REMOLON_FRONTEND_LIBRARY_DIRS ${Poco_Net_LIBRARY})
list(APPEND REMOLON_FRONTEND_LIBRARY_DIRS ${Poco_Foundation_LIBRARY})

# Pam includes
list(APPEND REMOLON_FRONTEND_INCLUDE_DIRS ${PAM_INCLUDE_DIRS})
# Pam library
list(APPEND REMOLON_FRONTEND_LIBRARY_DIRS ${PAM_LIBRARIES})

# c++ threading support
list(APPEND REMOLON_FRONTEND_LIBRARY_DIRS ${CMAKE_THREAD_LIBS_INIT})


target_include_directories(remolon_frontend_server PUBLIC ${REMOLON_FRONTEND_INCLUDE_DIRS} remolonUtil)
target_link_libraries(remolon_frontend_server ${REMOLON_FRONTEND_LIBRARY_DIRS} remolonutil)

# copy configuration file to binary directory
configure_file(${CMAKE_SOURCE_DIR}/frontendServerConfig.cfg ${CMAKE_CURRENT_BINARY_DIR}/frontendServerConfig.cfg COPYONLY)
file(COPY ${CMAKE_SOURCE_DIR}/client DESTINATION ${CMAKE_CURRENT_BINARY_DIR})