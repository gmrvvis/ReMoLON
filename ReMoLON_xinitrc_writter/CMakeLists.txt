list(APPEND REMOLON_XINITRC_SOURCES src/main.cpp)

add_executable(remolon_xinit_writter
  ${REMOLON_XINITRC_HEADERS}
  ${REMOLON_XINITRC_SOURCES}
)