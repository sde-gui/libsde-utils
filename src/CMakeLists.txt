#set(WHISKERMENU_FLAGS "-Wall -Wextra")
#if(ENABLE_WERROR)
#    set(WHISKERMENU_FLAGS "${WHISKERMENU_FLAGS} -Werror")
#endif()
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WHISKERMENU_FLAGS}")
#set(CMAKE_CXX_FLAGS "-fvisibility=hidden ${WHISKERMENU_FLAGS} ${CMAKE_CXX_FLAGS}")
#if(ENABLE_AS_NEEDED)
#    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_SHARED_LINKER_FLAGS}")
#endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(glib2 REQUIRED glib2>=2.36)


#add_definitions(
#    -DGETTEXT_PACKAGE="xfce4-whiskermenu-plugin"
#    -DLOCALEDIR="${CMAKE_INSTALL_FULL_LOCALEDIR}"
#    -DGSEAL_ENABLE=1
#    -DGDK_DISABLE_SINGLE_INCLUDES=1
#    -DGDK_PIXBUF_DISABLE_SINGLE_INCLUDES=1
#    -DGTK_DISABLE_SINGLE_INCLUDES=1
#    -DGTK_MULTIHEAD_SAFE=1
#    ${exo_CFLAGS_OTHER}
#    ${garcon_CFLAGS_OTHER}
#    ${lxpanelx_CFLAGS_OTHER}
#    ${libxfce4ui_CFLAGS_OTHER}
#    ${libxfce4util_CFLAGS_OTHER})

add_definitions(
    ${glib2_CFLAGS_OTHER})


add_library(sde-util MODULE
    strings.c
)

#    configuration_dialog.cpp

target_link_libraries(sde-util
    ${glib2_LIBRARIES})

install(TARGETS sde-util LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/)

