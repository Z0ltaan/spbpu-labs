cmake_minimum_required(VERSION 3.22)

project(course)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory(mathlib)

add_executable(course main.cpp)

target_include_directories(course PRIVATE "${PROJECT_SOURCE_DIR}/mathlib")

target_link_libraries(course mathlib)
