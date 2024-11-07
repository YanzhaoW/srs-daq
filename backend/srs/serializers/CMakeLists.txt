target_include_directories(srscpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(srscpp PRIVATE zpp_bits::zpp_bits)
