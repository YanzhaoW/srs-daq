configure_file(${CMAKE_SOURCE_DIR}/doc/conf.py.in ${CMAKE_BINARY_DIR}/conf.py)

add_custom_target(
    sphinx
    COMMENT "launching sphinx build"
    COMMAND sphinx-build --build ${SPHINX_BUILDER} --write-all -c ${CMAKE_BINARY_DIR}
            ${CMAKE_SOURCE_DIR}/doc ${CMAKE_BINARY_DIR}/sphinx)
