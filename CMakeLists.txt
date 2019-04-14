cmake_minimum_required(VERSION 3.10)
project(Mukyun-Engine)

option(MUKYUN_BINARY "Compiles mukyun_bin" ON)
option(MUKYUN_CPPCHECK "Runs CppCheck" OFF)

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK AND MUKYUN_CPPCHECK)
	message("-- CppCheck found ${CMAKE_CXX_CPPCHECK}")
	list(
			APPEND CMAKE_CXX_CPPCHECK
			"--enable=warning,style,performance"
			"--suppress=*:${CMAKE_SOURCE_DIR}/libs/*"
			"-q"
			"-j8"
	)
elseif(NOT MUKYUN_CPPCHECK)
	message("-- CppCheck not activated")
	set(CMAKE_CXX_CPPCHECK "")
else()
	message("-- CppCheck not found")
	set(CMAKE_CXX_CPPCHECK "")
endif()

add_subdirectory(mukyunengine)

if (MUKYUN_BINARY)
	add_subdirectory(mukyunbin)
	file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/mukyunbin)
	file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR}/mukyunbin)
endif ()