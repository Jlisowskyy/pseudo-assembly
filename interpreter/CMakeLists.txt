cmake_minimum_required(VERSION 3.26)
project(interpreter C)

set(CMAKE_C_STANDARD 11)

add_executable(interpreter main.c
        include/interpreterCLI.h
        src/interpreterCLI.c
        include/errors.h
        src/errors.c
        include/token.h
        include/interpreter.h
        src/interpreter.c
        include/list.h
        src/list.c
        include/lexer.h
        src/lexer.c
        include/compMacros.h)
