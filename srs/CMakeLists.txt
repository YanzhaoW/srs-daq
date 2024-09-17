add_library(srs SHARED)

target_sources(
    srs
    PRIVATE Application.cpp
            utils/Connections.cpp
            analysis/DataProcessor.cpp
            analysis/DataStructs.cpp
            analysis/DataWriter.cpp)

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

target_include_directories(
    srs
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/devices ${CMAKE_CURRENT_SOURCE_DIR}/utils
            ${CMAKE_CURRENT_SOURCE_DIR}/analysis
    PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(
    srs
    PUBLIC asio::asio gsl::gsl-lite
    PRIVATE fmt::fmt
            zpp_bits::zpp_bits
            spdlog::spdlog
            onetbb::onetbb
            CLI11::CLI11)

if(ROOT_FOUND)
    target_link_libraries(srs PRIVATE ROOT::RIO ROOT::Tree ROOT::Hist)
    target_compile_definitions(srs PRIVATE HAS_ROOT=1)
    target_compile_options(srs PRIVATE "-Wno-cpp")
    root_generate_dictionary(
        srs_dict
        analysis/DataStructs.hpp
        MODULE
        srs
        LINKDEF
        SRSLinkDef.hpp)
endif()
