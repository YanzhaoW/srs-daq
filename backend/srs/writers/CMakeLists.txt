target_sources(srscpp PRIVATE DataWriter.cpp)

target_link_libraries(srscpp PRIVATE glaze::glaze)
target_include_directories(srscpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(
    srscpp
    PRIVATE FILE_SET
            privateHeaders
            FILES
            BinaryFileWriter.hpp
            DataWriter.hpp
            DataWriterOptions.hpp
            JsonWriter.hpp
            RootFileWriter.hpp
            UDPWriter.hpp)
