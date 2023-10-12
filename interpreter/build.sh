#!/bin/sh
gcc -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-6 -pthread -O3 -Wall -Wextra -march=native src/errors.c src/interpreter.c src/interpreterCLI.c src/lexer.c src/list.c src/stack.c main.c -o interpreter -lglib-2.0 
