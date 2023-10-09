//
// Created by Jlisowskyy on 10/8/23.
//

#ifndef INTERPRETER_COMPMACROS_H
#define INTERPRETER_COMPMACROS_H

#include <inttypes.h>

#define DEBUG_

// ------------------------------------
// Machine specification settings
// ------------------------------------

#define MACHINE_REGISTERS 16
#define MACHINE_BASIC_TYPE int64_t
#define MACHINE_STR_TO_NUM_FUNC strtol // straightforward compatible only stroll and stroll

#endif //INTERPRETER_COMPMACROS_H
