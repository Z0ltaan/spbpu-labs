cmake_minimum_required(VERSION 3.22)
project(1course LANGUAGES CXX)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# collect all source files
file(GLOB_RECURSE source_files ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE imgui_files ${CMAKE_SOURCE_DIR}/imgui/*.cpp)

add_executable(${PROJECT_NAME} ${source_files} ${imgui_files})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL glfw3)
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_SOURCE_DIR}/include
  PUBLIC ${CMAKE_SOURCE_DIR}/imgui)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  add_custom_target(
    copy_compile_commands
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Copying compile_commands.json to ${CMAKE_SOURCE_DIR}")

  add_dependencies(${PROJECT_NAME} copy_compile_commands)
endif()
