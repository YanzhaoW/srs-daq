cmake_minimum_required(VERSION 3.28)

project(
    srs
    VERSION 0.0.2
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(USE_ROOT "Force to use ROOT dependency." OFF)
option(NO_ROOT "Disable the usage of ROOT dependency." OFF)

include(GNUInstallDirs)
include(cmake/project_config.cmake)

add_subdirectory(frontend)
add_subdirectory(backend)
