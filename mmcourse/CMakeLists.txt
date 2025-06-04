cmake_minimum_required(VERSION 3.22)
project(mmcourse)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(mathlib)

add_executable(mmcourse main.cpp
        labDefinitions.cpp)

target_include_directories(mmcourse PRIVATE "${PROJECT_SOURCE_DIR}/mathlib")
target_include_directories(mmcourse PRIVATE "${PROJECT_SOURCE_DIR}/Eigen")
target_link_libraries(mmcourse mathlib)