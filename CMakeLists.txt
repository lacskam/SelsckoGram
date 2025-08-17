cmake_minimum_required(VERSION 3.16)

project(selsckoGram LANGUAGES C)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/proto
    ${CMAKE_CURRENT_SOURCE_DIR}/erproc
)

add_executable(selsckoGram server.c
    erproc/erproc.h erproc/erproc.c
    proto/proto.h proto/proto.c)

include(GNUInstallDirs)
install(TARGETS selsckoGram
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
