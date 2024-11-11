set_target_properties(
    srs_data
    PROPERTIES OUTPUT_NAME srsdata
               EXPORT_NAME data
               SOVERSION ${PROJECT_VERSION_MAJOR}
               VERSION ${PROJECT_VERSION})

set_target_properties(
    srscpp
    PROPERTIES OUTPUT_NAME srs
               EXPORT_NAME srs
               SOVERSION ${PROJECT_VERSION_MAJOR}
               VERSION ${PROJECT_VERSION})

install(TARGETS srscpp EXPORT srs_export FILE_SET publicHeaders)
install(TARGETS srs_data EXPORT srs_export FILE_SET HEADERS)
install(TARGETS srs_control)

install(EXPORT srs_export DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/srs)
