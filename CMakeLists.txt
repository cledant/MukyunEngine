cmake_minimum_required(VERSION 3.6)
project(MukyunEngine)

#Global build type
set(CMAKE_BUILD_TYPE Release)

#Setup Glad
set(GLAD_API "gl=4.1")
add_subdirectory(libs/glad ${CMAKE_BINARY_DIR}/libs/glad/build)

#Setup GLFW
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_INSTALL "" OFF)
add_subdirectory(libs/glfw ${CMAKE_BINARY_DIR}/libs/glfw/build)

#Setup Assimp
option(BUILD_SHARED_LIBS OFF)
option(ASSIMP_NO_EXPORT ON)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
add_subdirectory(libs/assimp ${CMAKE_BINARY_DIR}/libs/assimp/build)

#Setup FreeType
add_subdirectory(libs/freetype2 ${CMAKE_BINARY_DIR}/libs/freetype2/build)

#Setup cpp version and Release type
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
		libs/freetype2/include
		${CMAKE_BINARY_DIR}/libs/assimp/build/include)
link_directories(${CMAKE_BINARY_DIR}/libs/glad/build
		${CMAKE_BINARY_DIR}/libs/glfw/build/src
		${CMAKE_BINARY_DIR}/libs/assimp/build/code
		${CMAKE_BINARY_DIR}/libs/freetype2/build)

#Setup Main Lib with all compilations flag
set(SOURCE_FILES
		srcs/WindowManager/Glfw_manager.cpp
		srcs/WindowManager/Window.cpp
		srcs/WindowManager/Input.cpp
		srcs/Exceptions/GeneralException.cpp
		srcs/OpenGL/RessourceManager.cpp
		srcs/OpenGL/oGL_utility.cpp
		srcs/OpenGL/Ressource/Shader.cpp
		srcs/OpenGL/Ressource/Mesh.cpp
		srcs/OpenGL/Ressource/Model.cpp
		srcs/OpenGL/Ressource/Texture.cpp
		srcs/OpenGL/RenderBin/ARenderBin.cpp
		srcs/OpenGL/RenderBin/ARenderBin_Based/BasicPropRenderBin.cpp
		srcs/OpenGL/RenderBin/ARenderBin_Based/BasicColorRenderBin.cpp
		srcs/GameEntities/Utility/CollisionBox.cpp
		srcs/Cameras/Camera.cpp
		srcs/GameEntities/Prop/AProp.cpp
		srcs/Light/ALight.cpp
		srcs/Light/ALight_Based/PointLight.cpp
		srcs/Light/ALight_Based/DirectionalLight.cpp
		srcs/Light/ALight_Based/SpotLight.cpp
		srcs/OpenGL/RenderBin/ARenderBin_Based/MultiPointDirSpotLightRenderBin.cpp
		srcs/OpenGL/LightContainer/LightContainer.cpp
		srcs/OpenGL/RenderBin/ARenderBin_Based/DiffuseColored.cpp
		srcs/OpenGL/Ressource/AFramebuffer.cpp
		srcs/OpenGL/Ressource/AFramebuffer_Based/ImageFramebuffer.cpp
		srcs/OpenGL/Ressource/AFramebuffer_Based/DirectionalDepthMap.cpp
		srcs/OpenGL/RenderBin/ARenderBin_Based/AShadowRenderBin.cpp
		srcs/OpenGL/Shadow/ShadowRenderer.cpp
		srcs/OpenGL/RenderBin/AShadowRenderBin_Based/MultiPointDirLightShadowRenderBin.cpp
		srcs/UI/ShaderSurface/ShaderSurface.cpp
		srcs/UI/ShaderSurface/TextureShaderBuffer.cpp
		srcs/OpenGL/Ressource/AFramebuffer_Based/OmnidirectionalDepthMap.cpp
		srcs/Engine/Engine.cpp
		srcs/OpenGL/Ressource/Fontset.cpp
		srcs/GameEntities/Prop/Prop.cpp)
add_library(engine STATIC ${SOURCE_FILES})
target_link_libraries(engine glad glfw assimp freetype)
target_compile_options(engine PRIVATE -Wall -Wextra)

#Creating executable
set(SOURCE_FILES_MAIN
		srcs/main.cpp
		srcs/Env/Init/ShaderLoading/ShaderLoading.cpp
		srcs/Env/Init/EngineInit.cpp
		srcs/Env/Init/SceneInit/InitTestInstancing.cpp
		srcs/Env/Init/SceneInit/InitTestMultiLight.cpp
		srcs/Env/Init/SceneInit/InitTestDirectionalShadow.cpp
		srcs/Env/Init/SceneInit/InitTestOmniShadow.cpp
		srcs/Env/Init/SceneInit/InitTestSpotShadow.cpp
		srcs/Env/Env.cpp)
add_executable(mukyun_engine ${SOURCE_FILES_MAIN})
target_compile_options(mukyun_engine PRIVATE -Wall -Wextra)

#Build Order
add_dependencies(engine glad glfw assimp freetype)
add_dependencies(mukyun_engine engine)
target_link_libraries(mukyun_engine glad glfw assimp freetype engine)

#Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
