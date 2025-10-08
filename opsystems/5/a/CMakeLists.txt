cmake_minimum_required(VERSION 3.22)
project(lab5 LANGUAGES C)

# file(GLOB source_files src/*.c)
# add_library(source_files INTERFACE source_files_list)

add_executable(client src/client.c)
add_executable(server src/server.c)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/include)

# target_link_libraries(${PROJECT_NAME}
#   PUBLIC
# )
