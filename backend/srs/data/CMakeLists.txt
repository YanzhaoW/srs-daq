if(BUILD_STATIC)
    add_library(srs_data STATIC)
else()
    add_library(srs_data SHARED)
endif()

target_link_libraries(
    srs_data
    PUBLIC protobuf::libprotobuf fmt::fmt
    PRIVATE Boost::thread)

target_sources(
    srs_data
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${CMAKE_SOURCE_DIR}/backend
           ${CMAKE_BINARY_DIR}/backend
           FILES
           DataStructs.hpp
           DataStructsFormat.hpp
           ${CMAKE_CURRENT_BINARY_DIR}/message.pb.h)

protobuf_generate(
    TARGET
    srs_data
    PROTOS
    message.proto
    PROTOC_OUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate(
    TARGET
    srs_data
    LANGUAGE
    python
    PROTOS
    message.proto
    PROTOC_OUT_DIR
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if(ROOT_FOUND)
    target_link_libraries(srs_data PUBLIC ROOT::RIO ROOT::Tree)
    target_compile_definitions(srs_data PUBLIC HAS_ROOT=1)
    target_compile_options(srs_data PUBLIC "-Wno-cpp")
    root_generate_dictionary(
        srs_dict
        DataStructs.hpp
        MODULE
        srs_data
        LINKDEF
        SRSLinkDef.hpp)
endif()
