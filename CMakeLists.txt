cmake_minimum_required(VERSION 3.28)

project(
    srs
    VERSION 0.0.3
    LANGUAGES CXX C)

include(cmake/install_packages.cmake)
include(GNUInstallDirs)
include(cmake/project_config.cmake)
include(cmake/option_settings.cmake)
include(cmake/check_compilers.cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND CMAKE_CXX_FLAGS "-fexperimental-library")
endif()

list(APPEND CMAKE_PREFIX_PATH ENV${CONDA_PREFIX})

add_subdirectory(frontend)
add_subdirectory(backend)
add_subdirectory(test)
