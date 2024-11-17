cmake_minimum_required(VERSION 3.28)

project(
    srs
    VERSION 0.1.0
    LANGUAGES CXX C)

include(${CMAKE_SOURCE_DIR}/cmake/install_config.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/install_packages.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/project_config.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/option_settings.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/check_compilers.cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND CMAKE_CXX_FLAGS "-fexperimental-library")
endif()

add_subdirectory(frontend)
add_subdirectory(backend)
add_subdirectory(test)

include(${CMAKE_SOURCE_DIR}/cmake/install_targets.cmake)
