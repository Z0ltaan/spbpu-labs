cmake_minimum_required(VERSION 3.22)

project(
  modelin1
  VERSION 0.1.0
  LANGUAGES CXX)

file(GLOB_RECURSE source CONFIGURE_DEPENDS ${CMAKE_HOME_DIRECTORY}/src/*.cpp)

file(COPY "${CMAKE_HOME_DIRECTORY}/resources" 
  DESTINATION "${CMAKE_BINARY_DIR}")

add_executable(${PROJECT_NAME} ${source})
# include(FetchContent)
#
# FetchContent_Declare( raygui GIT_REPOSITORY
# https://github.com/raysan5/raygui.git GIT_TAG
# 020a61bebcbe288b4414de3416e219ef40af847a SOURCE_DIR
# "${CMAKE_HOME_DIRECTORY}/external/raygui" )
#
# FetchContent_Declare( raygui-cpp GIT_REPOSITORY
# https://github.com/scastd/raygui-cpp.git GIT_TAG
# 091729ec01ba00b18c9a7b5acd29123abf906479 SOURCE_DIR
# "${CMAKE_HOME_DIRECTORY}/external/raygui-cpp" )
#
# FetchContent_MakeAvailable(raygui raygui-cpp)
#
# add_subdirectory("${CMAKE_HOME_DIRECTORY}/external/raygui-cpp")

find_package(glfw3 CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)
# find_package(raygui CONFIG REQUIRED)
find_path(RAYLIB_CPP_INCLUDE_DIRS "raylib-cpp.hpp")
find_path(RAYGUI_INCLUDE_DIRS "raygui.h")
target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${RAYLIB_CPP_INCLUDE_DIRS}
  # PRIVATE ${CMAKE_HOME_DIRECTORY}/external/raygui
  PRIVATE ${RAYGUI_INCLUDE_DIRS}
  PRIVATE ${CMAKE_HOME_DIRECTORY}/external/raylib-extra)
target_link_libraries(modelin1 PRIVATE raylib glfw)
