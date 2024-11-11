set_target_properties(
    srs_data
    PROPERTIES OUTPUT_NAME srsdata
               EXPORT_NAME data
               SOVERSION ${PROJECT_VERSION_MAJOR}
               VERSION ${PROJECT_VERSION})

# set_target_properties(
#     srscpp
#     PROPERTIES OUTPUT_NAME srs
#                EXPORT_NAME srs
#                SOVERSION ${PROJECT_VERSION_MAJOR}
#                VERSION ${PROJECT_VERSION})

# install(
#     TARGETS srscpp
#     EXPORT srs_export
#     FILE_SET publicHeaders)
install(
    TARGETS srs_data
    EXPORT srs_export
    FILE_SET HEADERS)

install(EXPORT srs_export DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/srs)

install(TARGETS srs_control RUNTIME_DEPENDENCY_SET appDeps)

install(PROGRAMS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/srs_check_udp
                 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/srs_check_binpb
        DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/message_pb2.py DESTINATION ${CMAKE_INSTALL_BINDIR})

install(
    RUNTIME_DEPENDENCY_SET
    appDeps
    PRE_EXCLUDE_REGEXES
    [[libc\.so\..*]]
    [[libgcc_s\.so\..*]]
    [[libm\.so\..*]]
    [[libstdc\+\+\.so\..*]]
    [[ld-linux-x86-64\.so\..*]])
