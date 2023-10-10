project(info_sec_lab6)

cmake_minimum_required(VERSION 2.8)

set(HEADERS huffman.h binarytree.h frequency_table.h utils.h)
set(SOURCES main.cpp huffman.cpp binarytree.cpp utils.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

