add_library(srs_data SHARED)
target_link_libraries(srs_data PUBLIC protobuf::libprotobuf)

target_include_directories(srs_data PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
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
        srscpp
        LINKDEF
        SRSLinkDef.hpp)
endif()
