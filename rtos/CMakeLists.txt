cmake_minimum_required(VERSION 3.22)

project(
  TotalOSKernel
  VERSION 0.1.0
  LANGUAGES C)

file(GLOB_RECURSE source_files CONFIGURE_DEPENDS
     ${CMAKE_HOME_DIRECTORY}/src/*.c)

add_library(${PROJECT_NAME} ${source_files})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_HOME_DIRECTORY}/include/)
