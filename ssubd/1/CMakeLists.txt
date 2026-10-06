cmake_minimum_required(VERSION 3.22)

project(ssubd LANGUAGES CXX)

file(GLOB_RECURSE source_list ${CMAKE_HOME_DIRECTORY}/src/*.cpp)

add_executable(${PROJECT_NAME} ${sourrce_list})
