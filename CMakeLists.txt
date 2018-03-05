cmake_minimum_required(VERSION 3.9)
project(MukyunEngine)

#Setup cpp version and flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Setup Glad
set(GLAD_API "gl=4.1")
add_subdirectory(libs/glad ${CMAKE_BINARY_DIR}/libs/glad/build)

#Setup GLFW
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_INSTALL "" OFF)
add_subdirectory(libs/glfw ${CMAKE_BINARY_DIR}/libs/glfw/build)

#Setup Assimp
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_NO_EXPORT ON)
add_subdirectory(libs/assimp ${CMAKE_BINARY_DIR}/libs/assimp/build)

set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -O3")

include_directories(includes
		libs/glm
		libs/stb
		libs/glad/build/includes
		libs/glfw/include/GLFW
		libs/assimp/include
		libs/assimp/build/include
		${CMAKE_BINARY_DIR}/libs/assimp/build/include)
link_directories(${CMAKE_BINARY_DIR}/libs/glad/build
		${CMAKE_BINARY_DIR}/libs/glfw/build/src
		${CMAKE_BINARY_DIR}/libs/assimp/build/code)
set(SOURCE_FILES
		srcs/main.cpp)

#OS specific libs
if (APPLE)
	find_library(OPEN_GL OpenGL REQUIRED)
	find_library(COCOA COCOA REQUIRED)
	find_library(IOKIT IOkit REQUIRED)
	find_library(COREVIDEO CoreVideo REQUIRED)
	set(EXTRA_LIBS ${OPEN_GL} ${COCOA} ${IOKIT} ${COREVIDEO})
endif ()

add_executable(engine ${SOURCE_FILES})
target_link_libraries(engine glad glfw assimp ${EXTRA_LIBS})
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})