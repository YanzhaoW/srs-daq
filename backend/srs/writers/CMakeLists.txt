target_sources(srs PRIVATE DataWriter.cpp)

target_link_libraries(srs PRIVATE glaze::glaze)
target_include_directories(srs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
