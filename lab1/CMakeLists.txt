cmake_minimum_required(VERSION 3.22)

project(stmc)

set (CMAKE_PREFIX_PATH /opt/systemc)
find_package(SystemCLanguage CONFIG REQUIRED)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED 17)

add_executable(stmc.exe main.cpp ImpulseGenRTL.cpp ImpulseGenTest.cpp)
target_link_libraries(stmc.exe SystemC::systemc)
