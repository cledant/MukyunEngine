option(BUILD_SHARED_LIBS OFF)
option(BUILD_STATIC_LIBS OFF)
option(GLM_TEST_ENABLE OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glm ${CMAKE_BINARY_DIR}/libs/glm/build)
