cmake_minimum_required(VERSION 3.10)
project(Binary-Mukyun-Engine)

add_executable(mukyun_bin
		srcs/main.cpp
		srcs/Env/Init/ShaderLoading/ShaderLoading.cpp
		srcs/Env/Init/EngineInit.cpp
		srcs/Env/Init/SceneInit/InitTestInstancing.cpp
		srcs/Env/Init/SceneInit/InitTestMultiLight.cpp
		srcs/Env/Init/SceneInit/InitTestDirectionalShadow.cpp
		srcs/Env/Init/SceneInit/InitTestOmniShadow.cpp
		srcs/Env/Init/SceneInit/InitTestSpotShadow.cpp
		srcs/Env/Env.cpp)
add_dependencies(mukyun_bin mukyunengine)
target_include_directories(mukyun_bin PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_compile_options(mukyun_bin PRIVATE -Wall -Wextra)
target_link_libraries(mukyun_bin PRIVATE mukyunengine)
set_target_properties(
		mukyun_bin
		PROPERTIES
		CMAKE_CXX_STANDARD 14
		CMAKE_CXX_STANDARD_REQUIRED ON
		CMAKE_CXX_EXTENSIONS OFF)