//
// Created by Jlisowskyy on 10/7/23.
//

#ifndef INTERPRETER_ERRORS_H
#define INTERPRETER_ERRORS_H

#include <stdlib.h>

void writeHelpOut(char *unrecognizedFile);
void throwStructuralError(const char* msg);
void throwError(const char* msg, size_t line);
void throwUnrecognizedCharError(size_t line, char unrecognized);

#endif //INTERPRETER_ERRORS_H
