cmake_minimum_required(VERSION 3.22)

project(lab3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory(mathlib)

add_executable(lab3 main.cpp
)

target_include_directories(lab3 PRIVATE "${PROJECT_SOURCE_DIR}/mathlib")

target_link_libraries(lab3 mathlib)
