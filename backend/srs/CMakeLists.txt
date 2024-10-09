# target_sources(srs PRIVATE Application.cpp utils/Connections.cpp)
target_sources(srs PRIVATE Application.cpp)

add_subdirectory(utils)
add_subdirectory(analysis)

target_sources(
    srs
    PRIVATE FILE_SET
            HEADERS
            BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
            FILES
            utils/Serializer.hpp
            utils/CommonDefitions.hpp
            devices/Fec.hpp
            devices/VmmDevice.hpp
            Application.hpp)

target_include_directories(srs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/devices)
