cmake_minimum_required(VERSION 3.22)

project(dbcourse LANGUAGES CXX)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(libpqxx REQUIRED)

file(GLOB_RECURSE source_files ${CMAKE_HOME_DIRECTORY}/src/*.cpp)
file(GLOB_RECURSE imgui_files ${CMAKE_HOME_DIRECTORY}/imgui/*.cpp)

add_executable(${PROJECT_NAME} ${source_files} ${imgui_files})
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_HOME_DIRECTORY}/imgui/
  PUBLIC ${CMAKE_HOME_DIRECTORY}/include/)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL glfw3 libpqxx::pqxx)
