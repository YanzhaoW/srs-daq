include(FetchContent)

fetchcontent_declare(
    glaze
    GIT_REPOSITORY https://github.com/stephenberry/glaze.git
    GIT_TAG main
    GIT_SHALLOW TRUE)

fetchcontent_makeavailable(glaze)

find_package(asio REQUIRED)
find_package(fmt REQUIRED)
find_package(zpp_bits REQUIRED)
find_package(gsl-lite REQUIRED)
find_package(spdlog REQUIRED)
find_package(CLI11 REQUIRED)
find_package(TBB REQUIRED)

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

add_library(srs SHARED)

add_subdirectory(srs)

target_include_directories(srs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(srs_control main.cpp)
target_link_libraries(srs_control PRIVATE srs spdlog::spdlog CLI11::CLI11)
