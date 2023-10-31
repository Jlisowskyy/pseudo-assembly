//
// Created by Jlisowskyy on 10/8/23.
//

#ifndef INTERPRETER_COMPMACROS_H
#define INTERPRETER_COMPMACROS_H

#include <inttypes.h>

// File used to preview and possibly modify some values used inside the machine

// ------------------------------
// Compilation settings
// ------------------------------

//#define DEBUG_

// defining DEBUG_ turns on token list printing after lexical analysis and probably other tools

// ------------------------------------
// Machine specification settings
// ------------------------------------

#define MACHINE_REGISTERS 16
// chosen to be able to hold 64bit pointers
#define MACHINE_BASIC_INT_TYPE int64_t
// straightforward compatible only strtoll and strtol
#define MACHINE_STR_TO_NUM_FUNC strtol
// TODO: future use of stack based command (POP, PUSH)
#define MACHINE_STACK_SIZE_UNITS 2048 // 16kb of stack 16 * 1024 / sizeof(MACHINE_BASIC_INT_TYPE)
// in bytes
#define MACHINE_MEMORY_SIZE_BYTES 16384

#endif //INTERPRETER_COMPMACROS_H
