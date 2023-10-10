cmake_minimum_required(VERSION 3.5)
project(des LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(
	-Werror

	-Wall
	-Wextra
	-Wpedantic
)

set(
	HEADERS
	des.hpp
)

set(
	SOURCES
	des.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${HEADERS} ${SOURCES})
