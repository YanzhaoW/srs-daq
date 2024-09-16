cmake_minimum_required(VERSION 3.26)

project(
    srs
    VERSION 0.0.1
    LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(asio REQUIRED)
find_package(fmt REQUIRED)
find_package(zpp_bits REQUIRED)
find_package(gsl-lite REQUIRED)
find_package(spdlog REQUIRED)
find_package(CLI11 REQUIRED)
find_package(TBB REQUIRED)

add_subdirectory(srs)
add_subdirectory(apps)
