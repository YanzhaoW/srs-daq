configure_file(${CMAKE_CURRENT_SOURCE_DIR}/check_udp_message.py
               ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/srs_check_udp COPYONLY USE_SOURCE_PERMISSIONS)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/check_binpb_message.py
               ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/srs_check_binpb COPYONLY USE_SOURCE_PERMISSIONS)

