add_subdirectory(backend)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/test_data.bin
               ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_data.bin COPYONLY USE_SOURCE_PERMISSIONS)
