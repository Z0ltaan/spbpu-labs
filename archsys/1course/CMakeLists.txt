cmake_minimum_required(VERSION 3.22)
project(1course LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(BUILD_TESTS OFF)

include(cmake/functions.cmake)
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_BASE_DIR ${CMAKE_HOME_DIRECTORY}/deps)

#collect all source files
file(GLOB source_files ${CMAKE_SOURCE_DIR}/src/*.cpp)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  add_custom_target(copy_compile_commands
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Copying compile_commands.json to ${CMAKE_SOURCE_DIR}"
  )
endif()

print(BUILD_TESTS)
if(BUILD_TESTS)
  message("Tests will be built...\n")
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY git@github.com:google/googletest.git
    GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723
  )
  FetchContent_MakeAvailable(googletest)
  set(INSTALL_GTEST OFF)

  #get list without main
  list(LENGTH source_files source_length)
  list(SUBLIST source_files 0 ${source_length} src_no_main)
  list(FILTER src_no_main EXCLUDE REGEX main.cpp)

  #collect all test files
  file(GLOB test_files ${CMAKE_SOURCE_DIR}/test/*.cpp)

  # setup tests
  enable_testing()
  add_executable(
    ${PROJECT_NAME}-test
    ${test_files}
    ${src_no_main}
  )
  target_link_libraries(
    ${PROJECT_NAME}-test
    gtest_main
    gmock
  )
  target_include_directories(
    ${PROJECT_NAME}-test
    PUBLIC ${CMAKE_SOURCE_DIR}/include/
    PUBLIC ${CMAKE_SOURCE_DIR}/test/include/
  )
  include(GoogleTest)
  gtest_discover_tests(${PROJECT_NAME}-test)
else()
    message("Tests won't be built...\n")
endif()

# setup main program
add_executable(
  ${PROJECT_NAME}
  ${source_files}
)
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_SOURCE_DIR}/include/
)

