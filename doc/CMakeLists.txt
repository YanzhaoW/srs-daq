configure_file(${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in ${CMAKE_CURRENT_BINARY_DIR}/conf.py)

find_package(Doxygen REQUIRED COMPONENTS dot)

set(DOXYGEN_GENERATE_XML YES)
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_EXTRACT_PRIVATE YES)
set(DOXYGEN_EXTRACT_PRIV_VIRTUAL YES)
set(DOXYGEN_EXTRACT_PACKAGE YES)
set(DOXYGEN_EXTRACT_STATIC YES)

doxygen_add_docs(
    doxygen ${PROJECT_SOURCE_DIR}/backend ${PROJECT_SOURCE_DIR}/examples ${PROJECT_SOURCE_DIR}/test
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Generate doxygen documentation")

add_custom_target(
    doc
    DEPENDS doxygen
    COMMENT "launching sphinx build"
    COMMAND sphinx-build --build ${SPHINX_BUILDER} --write-all -c ${CMAKE_CURRENT_BINARY_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
