cmake_minimum_required(VERSION 3.22)

project(time_is)

find_package(Boost REQUIRED COMPONENTS asio)

file(GLOB_RECURSE smtp_source_files
     ${CMAKE_HOME_DIRECTORY}/src/server/*.cpp)
add_executable(server ${smtp_source_files})
target_link_libraries(server PRIVATE Boost::asio)
target_compile_features(server PUBLIC cxx_std_20)

file(GLOB_RECURSE client_src ${CMAKE_HOME_DIRECTORY}/src/client/*.cpp)
add_executable(client ${client_src})
target_link_libraries(client PRIVATE Boost::asio)
target_compile_features(client PUBLIC cxx_std_20)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  add_custom_target(
    copy_compile_commands
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Copying compile_commands.json to ${CMAKE_SOURCE_DIR}")

  add_dependencies(server copy_compile_commands)
endif()
