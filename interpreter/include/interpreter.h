//
// Created by Jlisowskyy on 10/7/23.
//

#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include "token.h"
#include "list.h"

#define DECL_INSTRUCTION_FOUND 1
#define NON_DECL_INSTRUCTION_FOUND 0

// main machine loops
void interpFileTokens(list_t tokens);
void interpCLTokens(list_t tokens);

// processing function
inline int isDeclInstruction(const char *ident);
inline void processLabel();
inline void processIdent();
inline void addToDataTable(node_t* p);
inline void addToCodeTable(node_t* p);

// helping function
void printInterpState();

#endif //INTERPRETER_INTERPRETER_H

