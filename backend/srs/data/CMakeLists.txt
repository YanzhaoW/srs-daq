target_link_libraries(srs PRIVATE protobuf::protobuf)

protobuf_generate(
    TARGET
    srs
    PROTOS
    message.proto
    PROTOC_OUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR})

target_sources(
    srs
    PRIVATE FILE_SET
            HEADERS
            BASE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
            FILES
            message.pd.h)
