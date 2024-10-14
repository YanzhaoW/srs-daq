target_include_directories(srs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(srs PRIVATE zpp_bits::zpp_bits)
