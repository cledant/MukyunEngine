cmake_minimum_required(VERSION 3.10)
project(Lib-Mukyun-Engine)

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(freetype REQUIRED)
find_package(glm REQUIRED)
find_package(stb REQUIRED)

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
#add_dependencies(mukyunengine glad glfw assimp freetype glm)
target_include_directories(mukyunengine PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
		${MUKYUN_STB_DIR}
		${MUKYUN_FREETYPE_INCLUDES_DIR}
		${ASSIMP_INCLUDE_DIRS})
target_link_libraries(mukyunengine PUBLIC glm glad freetype assimp glfw)
target_compile_options(mukyunengine PRIVATE -Wall -Wextra -std=c++14)