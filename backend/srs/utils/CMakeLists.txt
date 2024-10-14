target_sources(srs PRIVATE Connections.cpp)

target_include_directories(srs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
    srs
    PUBLIC asio::asio gsl::gsl-lite
    PRIVATE fmt::fmt spdlog::spdlog)
