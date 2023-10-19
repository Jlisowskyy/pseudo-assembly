//
// Created by Jlisowskyy on 10/7/23.
//

#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <stdint.h>
#include "compMacros.h"

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

typedef struct token{
    char* strVal;
    tokenType_t type;
    MACHINE_BASIC_INT_TYPE numVal;
    uint32_t line;
}token_t;

#endif //INTERPRETER_TOKEN_H
