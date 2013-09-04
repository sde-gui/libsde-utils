
find_package(PkgConfig REQUIRED)
pkg_check_modules(glib2 REQUIRED glib-2.0>=2.36)

add_definitions(
    ${glib2_CFLAGS})


add_library(sde-utils MODULE
    strings.c
    paths.c
)

target_link_libraries(sde-utils
    ${glib2_LIBRARIES})

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${headers} DESTINATION include/sde-utils-1.0/sde-utils)

install(TARGETS sde-utils LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/)

