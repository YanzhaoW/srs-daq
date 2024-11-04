target_sources(srscpp PRIVATE DataProcessor.cpp DataStructs.cpp DataProcessManager.cpp)

target_link_libraries(srscpp PRIVATE onetbb::onetbb)
target_include_directories(srscpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(ROOT_FOUND)
    target_link_libraries(srscpp PRIVATE ROOT::RIO ROOT::Tree)
    target_compile_definitions(srscpp PRIVATE HAS_ROOT=1)
    target_compile_options(srscpp PRIVATE "-Wno-cpp")
    root_generate_dictionary(
        srs_dict
        DataStructs.hpp
        MODULE
        srscpp
        LINKDEF
        SRSLinkDef.hpp)
endif()
