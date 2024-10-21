find_package(asio REQUIRED)
find_package(fmt REQUIRED)
find_package(zpp_bits REQUIRED)
find_package(gsl-lite REQUIRED)
find_package(spdlog REQUIRED)
find_package(CLI11 REQUIRED)
find_package(TBB REQUIRED)
find_package(protobuf REQUIRED CONFIG)

if(NOT NO_ROOT)
    if(USE_ROOT)
        message(STATUS "Force to use ROOT depenedency!")
        find_package(
            ROOT REQUIRED
            COMPONENTS RIO
                       Tree
                       Hist
                       RHTTP
                       Gpad)
    else()
        find_package(
            ROOT
            COMPONENTS RIO
                       Tree
                       Hist
                       RHTTP
                       Gpad)
    endif()
else()
    message(STATUS "ROOT depenedency is disabled!")
endif()

add_library(srscpp STATIC)

add_subdirectory(srs)

target_include_directories(srscpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(srs_control main.cpp)
target_link_libraries(srs_control PRIVATE srscpp spdlog::spdlog CLI11::CLI11)
