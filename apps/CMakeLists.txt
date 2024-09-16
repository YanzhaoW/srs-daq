add_executable(srs_control main.cpp)
target_link_libraries(srs_control PRIVATE srs spdlog::spdlog CLI11::CLI11)
