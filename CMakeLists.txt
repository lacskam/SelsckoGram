cmake_minimum_required(VERSION 3.16)

project(tcpServer LANGUAGES C)

add_executable(tcpServer main.c
    erproc.h erproc.c
    proto.h proto.c)

include(GNUInstallDirs)
install(TARGETS tcpServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
