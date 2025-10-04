cmake_minimum_required(VERSION 3.22)
project(olab2 LANGUAGES C)

# file(GLOB source_files src/*.c)

add_executable(father src/father.c)
add_executable(son src/son.c)
add_executable(relation src/relation.c)

# target_include_directories(son PRIVATE ${CMAKE_SOURCE_DIR}/include)
# target_include_directories(father PRIVATE ${CMAKE_SOURCE_DIR}/include)
# target_include_directories(relation PRIVATE ${CMAKE_SOURCE_DIR}/include)

# target_link_libraries(${PROJECT_NAME}
#   PUBLIC
# )
