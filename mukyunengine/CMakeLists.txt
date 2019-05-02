cmake_minimum_required(VERSION 3.10)
project(Lib-Mukyun-Engine)

#Setup Glad
set(GLAD_API "gl=4.1")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glad ${CMAKE_BINARY_DIR}/libs/glad/build)

#Setup GLFW
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_INSTALL "" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw ${CMAKE_BINARY_DIR}/libs/glfw/build)

#Setup Assimp
option(BUILD_SHARED_LIBS OFF)
option(ASSIMP_NO_EXPORT ON)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp ${CMAKE_BINARY_DIR}/libs/assimp/build)

#Setup FreeType2
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2 ${CMAKE_BINARY_DIR}/libs/freetype2/build)

#Setup glm
option(BUILD_SHARED_LIBS OFF)
option(BUILD_STATIC_LIBS OFF)
option(GLM_TEST_ENABLE OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glm ${CMAKE_BINARY_DIR}/libs/glm/build)

#Setup lib compilation
add_library(mukyunengine STATIC
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
add_dependencies(mukyunengine glad glfw assimp freetype glm)
target_include_directories(mukyunengine PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/stb>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp/include>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/libs/assimp/build/include>)
target_link_libraries(mukyunengine PUBLIC glm glad glfw freetype assimp)
target_compile_options(mukyunengine PRIVATE -Wall -Wextra -std=c++14)