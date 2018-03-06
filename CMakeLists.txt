cmake_minimum_required(VERSION 3.9)
project(MukyunEngine)

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

#Setup cpp version and Release type
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Setup directories
include_directories(includes
		libs/glm
		libs/stb
		${CMAKE_BINARY_DIR}/libs/glad/build/include
		libs/glfw/include/GLFW
		libs/assimp/include
		libs/assimp/build/include
		${CMAKE_BINARY_DIR}/libs/assimp/build/include)
link_directories(${CMAKE_BINARY_DIR}/libs/glad/build
		${CMAKE_BINARY_DIR}/libs/glfw/build/src
		${CMAKE_BINARY_DIR}/libs/assimp/build/code)

#Setup Main Lib with all compilations flag
set(SOURCE_FILES
		srcs/WindowManager/Glfw_manager.cpp
		srcs/WindowManager/Window.cpp
		srcs/WindowManager/Input.cpp
		srcs/Exceptions/GeneralException.cpp
		srcs/OpenGL/RessourceManager.cpp
		srcs/OpenGL/oGL_utility.cpp
		srcs/OpenGL/Ressource/Shader.cpp
		srcs/OpenGL/Ressource/Framebuffer.cpp
		srcs/OpenGL/Ressource/Mesh.cpp
		srcs/OpenGL/Ressource/Model.cpp
		srcs/RenderBin/ARenderBin.cpp)
add_library(primary STATIC ${SOURCE_FILES})
target_compile_options(primary PRIVATE -Wall -Werror -Wextra)

#Setup secondary Lib with no compilation flag for unused var
#Used to fix stbi compilation
set(SOURCE_FILES_NO_FLAG
		srcs/OpenGL/Ressource/Texture.cpp)
add_library(secondary STATIC ${SOURCE_FILES_NO_FLAG})
target_compile_options(secondary PRIVATE -Wall -Wextra)

#Creating executable
set(SOURCE_FILES_MAIN
		srcs/main.cpp)
add_executable(engine ${SOURCE_FILES_MAIN})
target_compile_options(engine PRIVATE -Wall -Werror -Wextra)

#Build Order
add_dependencies(primary glad glfw assimp)
add_dependencies(secondary glad glfw assimp)
add_dependencies(engine primary secondary)
target_link_libraries(engine glad glfw assimp primary secondary)

#Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})