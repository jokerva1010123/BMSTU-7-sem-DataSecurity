cmake_minimum_required(VERSION 3.5)

project(lab_06 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
    HEADERS

    io/io.hpp
    huffman/huffman.hpp
    huffman/huffman_build_tree.hpp
    huffman/huffman_defines.hpp
)

set(
    SOURCES

    main.cpp

    io/io.cpp
    huffman/huffman.cpp
    huffman/huffman_build_tree.cpp
    )

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
