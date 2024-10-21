target_link_libraries(srscpp PRIVATE protobuf::protobuf)

protobuf_generate(
    TARGET
    srscpp
    PROTOS
    message.proto
    PROTOC_OUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR})

target_sources(
    srscpp
    PRIVATE FILE_SET
            HEADERS
            BASE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
            FILES
            message.pd.h)
