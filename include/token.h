//
// Created by Jlisowskyy on 10/7/23.
//

#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <stdint.h>
#include "compMacros.h"

// token used in lexing transformation of character based tokens

#define INSTRUCTION_COUNT 21
#define DATA_TYPES_COUNT 1
enum machineDataTypes {
    INTEGER
};

extern const char* machineDataTypesIdent[DATA_TYPES_COUNT];

typedef enum tokenType {
    UNKNOWN,
    OPER_SEP,
    IDENTIFIER,
    LABEL,
    CONSTANT,
    LINE_SEP,
    ARR_SIZE,
    ADDRESS_READ,
    INTEGER_TYPE,
} tokenType_t;

// TODO: use union instead of separate strVal and numVal

typedef struct token{
    // used to hold values like identifiers of
    char* strVal;
    tokenType_t type;
    // used to hold constant values as well register indexes
    MACHINE_BASIC_INT_TYPE numVal;
    // used only in debugging information on error detection
    uint32_t line;
}token_t;

#endif //INTERPRETER_TOKEN_H