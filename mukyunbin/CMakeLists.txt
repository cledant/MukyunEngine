cmake_minimum_required(VERSION 3.10)
project(Binary-Mukyun-Engine)

#Setup binary compilation
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
target_compile_options(mukyun_bin PRIVATE -Wall -Wextra -std=c++14)
target_link_libraries(mukyun_bin PRIVATE mukyunengine)
