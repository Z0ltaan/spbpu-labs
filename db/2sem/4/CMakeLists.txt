cmake_minimum_required(VERSION 3.22)

project(tpass)

add_library(${PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/true_pass.c)

target_include_directories(${PROJECT_NAME}
                           PRIVATE /usr/include/postgresql/16/server/)
