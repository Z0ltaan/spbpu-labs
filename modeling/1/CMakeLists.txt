cmake_minimum_required(VERSION 3.22)

project(
  modelin1
  VERSION 0.1.0
  LANGUAGES CXX)

file(GLOB_RECURSE source CONFIGURE_DEPENDS ${CMAKE_HOME_DIRECTORY}/src/*.cpp)

add_executable(${PROJECT_NAME} ${source})

find_package(glfw3 CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_path(RAYLIB_CPP_INCLUDE_DIRS "raylib-cpp.hpp")
target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_CPP_INCLUDE_DIRS})
target_link_libraries(modelin1 PRIVATE raylib glfw)
