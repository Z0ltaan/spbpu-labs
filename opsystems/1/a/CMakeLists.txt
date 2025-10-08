cmake_minimum_required(VERSION 3.22)
project(olab1 LANGUAGES C)

file(GLOB source_files src/*.c)
# add_library(source_files INTERFACE source_files_list)

add_executable(${PROJECT_NAME} ${source_files})
target_include_directories(${PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/include
)

# target_link_libraries(${PROJECT_NAME}
#   PUBLIC
# )
