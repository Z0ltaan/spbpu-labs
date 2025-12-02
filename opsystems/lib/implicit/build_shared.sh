#!/bin/bash

gcc -c -fPIC h_world.c
gcc -c -fPIC g_world.c

gcc -shared -o libworld.so h_world.o g_world.o

gcc -c main.c
gcc -o binary main.o -L. -lworld -Wl,-rpath,/home/georv/work/spbpu-labs/opsystems/lib/implicit
