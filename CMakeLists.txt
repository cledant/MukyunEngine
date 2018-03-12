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
		srcs/OpenGL/Ressource/Texture.cpp
		srcs/OpenGL/RenderBin/ARenderBin.cpp
		srcs/OpenGL/RenderBin/ARenderBin_Based/BasicPropRenderBin.cpp
		srcs/OpenGL/RenderBin/ARenderBin_Based/BasicColorRenderBin.cpp
		srcs/Interfaces/ICollidable.cpp
		srcs/Interfaces/IEntity.cpp
		srcs/Interfaces/ITransformable.cpp
		srcs/GameEntities/Utility/CollisionBox.cpp
		srcs/Cameras/Camera.cpp
		srcs/GameEntities/Prop.cpp
		srcs/TestInstancing/TestInstancing.cpp
		srcs/TestInstancing/InitTestInstancing.cpp
		srcs/Light/ALight.cpp
		srcs/Light/ALight_Based/PointLight.cpp
		srcs/Light/ALight_Based/DirectionalLight.cpp
		srcs/Light/ALight_Based/SpotLight.cpp
		srcs/OpenGL/RenderBin/ALightRenderBin.cpp
		srcs/OpenGL/RenderBin/ALightRenderBin_Based/MultiLightPointDirSpotLightRenderBin.cpp
		srcs/TestLight/TestLight.cpp
		srcs/TestLight/InitTestLight.cpp
		srcs/OpenGL/LightContainer/LightContainer.cpp)
add_library(primary STATIC ${SOURCE_FILES})
target_link_libraries(primary glad glfw assimp)
target_compile_options(primary PRIVATE -Wall -Wextra -g3)

#Creating executable
set(SOURCE_FILES_MAIN
		srcs/main.cpp)
add_executable(engine ${SOURCE_FILES_MAIN})
target_compile_options(engine PRIVATE -Wall -Wextra -g3)

#Build Order
add_dependencies(primary glad glfw assimp)
add_dependencies(engine primary)
target_link_libraries(engine glad glfw assimp primary)

#Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})