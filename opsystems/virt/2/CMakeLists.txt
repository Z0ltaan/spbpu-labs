cmake_minimum_required(VERSION 3.22)
project(virt)

add_executable(${PROJECT_NAME} sec.c)
add_executable(unset unset.c)
