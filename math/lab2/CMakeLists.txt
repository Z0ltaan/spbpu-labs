cmake_minimum_required(VERSION 3.22)

project(lab2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory(mathlib)

add_executable(lab2 main.cpp
        matrix.hpp)

target_include_directories(lab2 PRIVATE "${PROJECT_SOURCE_DIR}/mathlib")

target_link_libraries(lab2 mathlib)
