cmake_minimum_required(VERSION 3.22)

project(
  cmilan
  LANGUAGES CXX
  VERSION 0.5.0)

file(GLOB_RECURSE source_files CONFIGURE_DEPENDS
     ${CMAKE_HOME_DIRECTORY}/src/*.cpp)

add_executable(${PROJECT_NAME} ${source_files})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
