cmake_minimum_required(VERSION 3.10)
project(Mukyun-Engine)

include(CheckIPOSupported)
include(CheckCXXCompilerFlag)

option(MUKYUN_BINARY "Compiles mukyun_bin" ON)
option(MUKYUN_CPPCHECK "Runs CppCheck" OFF)
option(MUKYUN_LTO "Enables Link time optimization globaly" OFF)
option(MUKYUN_MARCH_NATIVE "Enables -march globaly" OFF)

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK AND MUKYUN_CPPCHECK)
	message("-- CppCheck found : ${CMAKE_CXX_CPPCHECK}")
	list(
			APPEND CMAKE_CXX_CPPCHECK
			"--enable=warning,style,performance"
			"--suppress=*:${CMAKE_SOURCE_DIR}/mukyunengine/libs/*"
			"-q"
			"-j8"
	)
elseif (NOT MUKYUN_CPPCHECK)
	message("-- CppCheck not activated")
	set(CMAKE_CXX_CPPCHECK "")
else ()
	message("-- CppCheck not found")
	set(CMAKE_CXX_CPPCHECK "")
endif ()

if (MUKYUN_MARCH_NATIVE)
	message("-- -march=native option activated")
	check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
	if (COMPILER_SUPPORTS_MARCH_NATIVE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
	endif ()
else ()
	message("-- -march=native option not activated")
endif ()

add_subdirectory(mukyunengine)

if (MUKYUN_BINARY)
	message("-- 	Generating mukyunbin")
	add_subdirectory(mukyunbin)
	file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/mukyunbin)
	file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR}/mukyunbin)
endif ()

if (MUKYUN_LTO)
	message("-- LTO option activated")
	check_ipo_supported(RESULT lto_result OUTPUT lto_output)
	if (lto_result)
		message("-- LTO option supported")
		set_target_properties(mukyunengine PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
		if (MUKYUN_BINARY)
			set_target_properties(mukyun_bin PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
		endif ()
	else ()
		message("-- LTO not supported : ${lto_output}")
	endif ()
else ()
	message("-- LTO option not activated")
endif ()