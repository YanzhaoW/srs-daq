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

# target_sources(
#     srscpp
#     PRIVATE FILE_SET
#             HEADERS
#             BASE_DIRS
#             ${CMAKE_CURRENT_BINARY_DIR}
#             FILES
#             ${CMAKE_CURRENT_BINARY_DIR}/message.pd.h)
