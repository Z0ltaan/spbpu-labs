cmake_minimum_required(VERSION 3.22)

project(synchrolab LANGUAGES C)

add_executable(server ${CMAKE_SOURCE_DIR}/src/server.c)
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(client ${CMAKE_SOURCE_DIR}/src/client.c)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/include)

