//
// Created by Jlisowskyy on 10/12/23.
//

#ifndef INTERPRETER_STACK_H
#define INTERPRETER_STACK_H

#include <stdlib.h>

// TODO: not used yet

typedef struct ch_stack{
    char* arr;
    size_t sizeMax;
    size_t used;
}ch_stack_t;

typedef struct stack64{
    void* arr;
    size_t sizeMax;
    size_t used;
}stack64_t;

#endif //INTERPRETER_STACK_H
