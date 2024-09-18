cmake_minimum_required(VERSION 3.26)

project(
    srs
    VERSION 0.0.1
    LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)
include(cmake/project_config.cmake)

include(FetchContent)

FetchContent_Declare(
  glaze
  GIT_REPOSITORY https://github.com/stephenberry/glaze.git
  GIT_TAG main
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glaze)

find_package(asio REQUIRED)
find_package(fmt REQUIRED)
find_package(zpp_bits REQUIRED)
find_package(gsl-lite REQUIRED)
find_package(spdlog REQUIRED)
find_package(CLI11 REQUIRED)
find_package(TBB REQUIRED)
find_package(ROOT COMPONENTS RIO Tree Hist)

add_subdirectory(source)
