# target_sources(srs PRIVATE Application.cpp utils/Connections.cpp)
target_sources(srscpp PRIVATE Application.cpp)

add_subdirectory(utils)
add_subdirectory(analysis)
add_subdirectory(serializers)
add_subdirectory(writers)
add_subdirectory(data)

target_sources(
    srscpp
    PRIVATE FILE_SET
            HEADERS
            BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
            FILES
            serializers/SerializableBuffer.hpp
            utils/CommonDefitions.hpp
            devices/Fec.hpp
            devices/VmmDevice.hpp
            Application.hpp)

target_include_directories(srscpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/devices)
target_link_libraries(srscpp PRIVATE srs_data)
