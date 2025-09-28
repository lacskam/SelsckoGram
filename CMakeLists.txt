cmake_minimum_required(VERSION 3.16)

project(selsckoGram LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)

find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIRS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/proto
    ${CMAKE_CURRENT_SOURCE_DIR}/erproc
)

add_executable(selsckoGram server.c
    erproc/erproc.h erproc/erproc.c
    proto/proto.h proto/proto.c
    proto/packet_type.c
    proto/packet_type.h
    proto/selsc_client/selsc_client.c
    proto/selsc_client/selsc_client.h
    selscserv_processing/selscservprocessing.h selscserv_processing/selscservprocessing.c
    database.h
    database.c
    )

include(GNUInstallDirs)
install(TARGETS selsckoGram
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(selsckoGram PRIVATE
    ${GLIB_LIBRARIES}
    ${PostgreSQL_LIBRARIES}  # ← ЭТОЙ СТРОКИ НЕ БЫЛО!
)
target_include_directories(selsckoGram PRIVATE ${GLIB_INCLUDE_DIRS})
target_compile_options(selsckoGram PRIVATE ${GLIB_CFLAGS} ${GLIB_CFLAGS_OTHER})


