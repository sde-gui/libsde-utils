cmake_minimum_required(VERSION 2.8)

project(libsde-utils)

option(ENABLE_WERROR "Enable -Werror flag for development" OFF)

include(GNUInstallDirs)

add_subdirectory(src)

