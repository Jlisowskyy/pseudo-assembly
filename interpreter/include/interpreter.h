//
// Created by Jlisowskyy on 10/7/23.
//

#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include "token.h"
#include "list.h"

void printInterpState();
void interpFileTokens(list_t tokens);
void interpCLTokens(list_t tokens);

#endif //INTERPRETER_INTERPRETER_H
