cmake_minimum_required(VERSION 3.10)
project(Mukyun-Engine)

add_subdirectory(mukyunengine)
add_subdirectory(mukyunbin)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/mukyunbin)
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR}/mukyunbin)