#!/usr/bin/env bash

gcc -fPIC -Wall -Werror -I$(pg_config --includedir-server) -c true_pass.c
gcc -shared -o true_pass.so true_pass.o

