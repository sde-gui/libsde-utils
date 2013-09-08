set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
if(ENABLE_WERROR)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif()

set(CMAKE_SHARED_LINKER_FLAGS "-ldl -Wl,--no-undefined ${CMAKE_SHARED_LINKER_FLAGS}")

##############################################################################

find_package(PkgConfig REQUIRED)
pkg_check_modules(glib2 REQUIRED glib-2.0>=2.36)

add_definitions(
    ${glib2_CFLAGS}
)


add_library(sde-utils-1.0 SHARED
    strings.c
    paths.c
    proc.c
    launcher.c
    log.c
)

target_link_libraries(sde-utils-1.0
    ${glib2_LIBRARIES}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sde-utils-1.0/sde-utils)
install(FILES sde-utils.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sde-utils-1.0)
install(TARGETS sde-utils-1.0 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/)

