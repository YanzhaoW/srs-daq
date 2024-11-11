if(BUILD_STATIC)
    add_library(srs_data STATIC)
else()
    add_library(srs_data SHARED)
endif()

target_link_libraries(srs_data PUBLIC protobuf::libprotobuf)

target_sources(
    srs_data
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${CMAKE_SOURCE_DIR}/backend
           ${CMAKE_BINARY_DIR}/backend
           FILES
           DataStructs.hpp
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

target_sources(srscpp PRIVATE DataStructs.cpp)
if(ROOT_FOUND)
    target_link_libraries(srscpp PRIVATE ROOT::RIO ROOT::Tree)
    target_compile_definitions(srscpp PRIVATE HAS_ROOT=1)
    target_compile_options(srscpp PRIVATE "-Wno-cpp")
    root_generate_dictionary(
        srs_dict
        DataStructs.hpp
        MODULE
        srs_data
        LINKDEF
        SRSLinkDef.hpp)
endif()
