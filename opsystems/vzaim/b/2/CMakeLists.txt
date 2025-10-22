cmake_minimum_required(VERSION 3.22)
project(bpart2 LANGUAGES C)

add_executable(server src/server_fifo1.c)
add_executable(client src/client_fifo1.c)
