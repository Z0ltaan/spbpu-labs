#!/bin/bash

gcc -c h_world.c
gcc -c g_world.c

ar cr libworld.a h_world.o g_world.o

gcc -c main.c
gcc -o binary main.o -L. -lworld
