target_sources(srscpp PRIVATE Connections.cpp)

target_link_libraries(
    srscpp PRIVATE $<BUILD_LOCAL_INTERFACE:gsl::gsl-lite> $<BUILD_LOCAL_INTERFACE:Boost::thread>
                   $<BUILD_LOCAL_INTERFACE:fmt::fmt> $<BUILD_LOCAL_INTERFACE:spdlog::spdlog>)

target_sources(
    srscpp
    PRIVATE FILE_SET
            privateHeaders
            FILES
            CommonConcepts.hpp
            CommonDefitions.hpp
            CommonFunctions.hpp
            ConnectionBase.hpp
            ConnectionTypeDef.hpp
            Connections.hpp
            ValidData.hpp)

target_sources(srscpp PUBLIC FILE_SET publicHeaders FILES AppStatus.hpp)
