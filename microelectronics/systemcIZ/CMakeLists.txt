cmake_minimum_required(VERSION 3.22)

project(sciz)

set (CMAKE_PREFIX_PATH /opt/systemc)
find_package(SystemCLanguage CONFIG REQUIRED)
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})
set (CMAKE_CXX_STANDARD_REQUIRED ${SystemC_CXX_STANDARD_REQUIRED})

add_executable(sciz main.cpp controller.cpp controller_test.cpp)
target_link_libraries(sciz SystemC::systemc)
