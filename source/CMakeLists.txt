add_library(srs SHARED)

add_subdirectory(srs)

target_include_directories(srs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(srs_control main.cpp)
target_link_libraries(srs_control PRIVATE srs spdlog::spdlog CLI11::CLI11)
