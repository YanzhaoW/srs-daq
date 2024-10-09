add_custom_target(
    npm-build ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Build frontend react application"
    COMMAND npm install --silent && npm run --silent build)
