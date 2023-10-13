//
// Created by Jlisowskyy on 10/8/23.
//

#ifndef INTERPRETER_COMPMACROS_H
#define INTERPRETER_COMPMACROS_H

#include <inttypes.h>

/*  General TODO:
 * - add correct load/store syntax
 * - boost up performance in some points
 * - repair all errors (THERE WAS IMPORTANT ONE WITH NULL I FORGOT)
 * - RECONSIDER PASSING PTR TO SOME SHITTY PROGRAM XDDDDDDDDD
 * - add option to compile
 * - add preprocessing labels first to enable forward jumps
 * - add some definitions stack based structured
 */



#define DEBUG_

// ------------------------------------
// Machine specification settings
// ------------------------------------

#define MACHINE_REGISTERS 16
#define MACHINE_BASIC_INT_TYPE int64_t // chosen to be able to hold 64bit pointers
#define MACHINE_STR_TO_NUM_FUNC strtol // straightforward compatible only stroll and stroll
#define MACHINE_STACK_SIZE_UNITS 2048 // 16kb of stack 16 * 1024 / sizeof(MACHINE_BASIC_INT_TYPE)
#define MACHINE_MEMORY_SIZE_BYTES 16384 // bytes

#endif //INTERPRETER_COMPMACROS_H
