//
// Created by Jlisowskyy on 10/7/23.
//

#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <stdint.h>
#include "compMacros.h"

#define INSTRUCTION_COUNT 21

typedef enum tokenType {
    UNKNOWN,
    OPER_SEP,
    IDENTIFIER,
    LABEL,
    CONSTANT,
    LINE_SEP,
} tokenType_t;

typedef struct token{
    char* strVal;
    tokenType_t type;
    MACHINE_BASIC_INT_TYPE numVal;
    uint32_t line;
}token_t;

#endif //INTERPRETER_TOKEN_H
