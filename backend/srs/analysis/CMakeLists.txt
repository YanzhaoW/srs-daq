target_sources(srscpp PRIVATE DataProcessor.cpp DataProcessManager.cpp)

target_link_libraries(srscpp PRIVATE onetbb::onetbb)
target_include_directories(srscpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
