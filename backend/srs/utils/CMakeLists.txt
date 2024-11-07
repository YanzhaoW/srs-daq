target_sources(srscpp PRIVATE Connections.cpp)

target_include_directories(srscpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
    srscpp
    PUBLIC Boost::system Boost::thread gsl::gsl-lite
    PRIVATE fmt::fmt spdlog::spdlog)
