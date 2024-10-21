include(FetchContent)

fetchcontent_declare(
    glaze
    GIT_REPOSITORY https://github.com/stephenberry/glaze.git
    GIT_TAG main
    GIT_SHALLOW TRUE)

fetchcontent_makeavailable(glaze)
