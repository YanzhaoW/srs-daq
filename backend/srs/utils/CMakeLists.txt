target_sources(srscpp PRIVATE Connections.cpp)

target_link_libraries(
    srscpp
    PUBLIC Boost::thread gsl::gsl-lite
    PRIVATE fmt::fmt spdlog::spdlog)

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
