//
// Created by Jlisowskyy on 10/8/23.
//

#ifndef INTERPRETER_LEXER_H
#define INTERPRETER_LEXER_H

#include "list.h"

list_t convertPlainTextToTokens(char* plainInput);
list_t convertCLInputToTokens(list_t tokenStream, char* CLInput);

void resetLexerMachineStates();
inline void processBlank();
inline void processNewLine();
inline void processLabelSep();
inline void processOperSep();
inline void processNumeric();
inline void processIdentifier();
inline void processComment();

#endif //INTERPRETER_LEXER_H
