cmake_minimum_required(VERSION 3.22)

project(
  gl_box
  VERSION 0.1.0
  LANGUAGES CXX)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp CONFIG REQUIRED)
file(GLOB_RECURSE source CONFIGURE_DEPENDS ${CMAKE_HOME_DIRECTORY}/src/*.cpp)

add_executable(${PROJECT_NAME} ${source})

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE glm::glm
  PRIVATE OpenGL::GL
  PRIVATE GLEW::GLEW
  PRIVATE glfw
  PRIVATE assimp::assimp)

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_HOME_DIRECTORY}/external)
