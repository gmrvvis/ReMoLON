include_directories( ${PROJECT_SOURCE_DIR}
                     ${PROJECT_SOURCE_DIR}/ReMoLON
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/src
                     ${PROJECT_BINARY_DIR} 
                    )

set( XINITRC_WRITER_SOURCES  src/main.cpp )
common_application( xinitrc_writer )
