target_sources(srscpp PRIVATE DataProcessor.cpp DataProcessManager.cpp)

target_link_libraries(srscpp PRIVATE $<BUILD_LOCAL_INTERFACE:TBB::tbb>)

target_sources(
    srscpp
    PRIVATE FILE_SET
            privateHeaders
            FILES
            DataProcessManager.hpp
            DataProcessor.hpp)
