cmake_minimum_required(VERSION 3.10)
project(Mukyun-Engine)

option(MUKYUN_CPP_CHECK "Runs CppCheck" OFF)

#Setup Glad
set(GLAD_API "gl=4.1")
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glad ${CMAKE_BINARY_DIR}/libs/glad/build)

#Setup GLFW
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_INSTALL "" OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glfw ${CMAKE_BINARY_DIR}/libs/glfw/build)

#Setup Assimp
option(BUILD_SHARED_LIBS OFF)
option(ASSIMP_NO_EXPORT ON)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/assimp ${CMAKE_BINARY_DIR}/libs/assimp/build)

#Setup FreeTypeinclude
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/freetype2 ${CMAKE_BINARY_DIR}/libs/freetype2/build)

#Cppcheck config
#find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
#if (CMAKE_CXX_CPPCHECK AND MUKYUN_CPP_CHECK)
#	list(
#			APPEND CMAKE_CXX_CPPCHECK
#			"--suppress=*:*libs/*"
#			"-j8"
#	)
#endif()

#Engine Lib
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
target_include_directories(mukyunengine PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libs/glm>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libs/stb>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/libs/glad/build/include>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libs/glfw/include/GLFW>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libs/assimp/include>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libs/assimp/build/include>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libs/freetype2/include>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/libs/assimp/build/include>)
target_link_libraries(mukyunengine glad glfw assimp freetype)
target_compile_options(mukyunengine PRIVATE -Wall -Wextra)
add_dependencies(mukyunengine glad glfw assimp freetype)
set_target_properties(
		mukyunengine
		PROPERTIES
		CMAKE_CXX_STANDARD 14
		CMAKE_CXX_STANDARD_REQUIRED ON
		CMAKE_CXX_EXTENSIONS OFF)

#Binary
add_executable(mukyun_engine
	srcs/main.cpp
	srcs/Env/Init/ShaderLoading/ShaderLoading.cpp
	srcs/Env/Init/EngineInit.cpp
	srcs/Env/Init/SceneInit/InitTestInstancing.cpp
	srcs/Env/Init/SceneInit/InitTestMultiLight.cpp
	srcs/Env/Init/SceneInit/InitTestDirectionalShadow.cpp
	srcs/Env/Init/SceneInit/InitTestOmniShadow.cpp
	srcs/Env/Init/SceneInit/InitTestSpotShadow.cpp
	srcs/Env/Env.cpp)
target_compile_options(mukyun_engine PRIVATE -Wall -Wextra)
add_dependencies(mukyun_engine mukyunengine)
target_link_libraries(mukyun_engine glad glfw assimp freetype mukyunengine)
set_target_properties(
	mukyun_engine
	PROPERTIES
	CMAKE_CXX_STANDARD 14
	CMAKE_CXX_STANDARD_REQUIRED ON
	CMAKE_CXX_EXTENSIONS OFF)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})