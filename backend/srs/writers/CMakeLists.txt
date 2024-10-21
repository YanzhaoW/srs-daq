target_sources(srscpp PRIVATE DataWriter.cpp)

target_link_libraries(srscpp PRIVATE glaze::glaze)
target_include_directories(srscpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
