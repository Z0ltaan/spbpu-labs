cmake_minimum_required(VERSION 3.22)
project(bpart3 LANGUAGES C)
add_executable(server src/server_fifomsg.c)
add_executable(client src/client_fifomsg.c)
