cmake_minimum_required(VERSION 3.5)

project(lab_04 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(
    HEADERS
    key_gen/key_gen.hpp
    io/io.hpp
    rsa/rsa.hpp
)

set(
    SOURCERS
    main.cpp

    key_gen/key_gen.cpp
    io/io.cpp
    rsa/rsa.cpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCERS})
target_link_libraries(${PROJECT_NAME} -lgmpxx -lgmp)
