//
// Created by Jlisowskyy on 10/7/23.
//
#include <stdio.h>

#include "../include/errors.h"
#include "../include/compMacros.h"

void writeHelpOut(char *unrecognizedFile) {
    fprintf(stderr, "[ERROR] Unrecognized file: %s. Expected usage:\n\t\"pas-int FILE_NAME\" to interpret from file\n", unrecognizedFile);
    fprintf(stderr, "\t\"pas-int\" to run interactive mode with all resources on display\n");
}

void throwStructuralError(const char *msg) {
    fprintf(stderr, "[ERROR] Encountered structural backend error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void throwError(const char *msg, size_t line) {
    fprintf(stderr, "[ERROR] Encountered error on line: %zu\n", line);
    fprintf(stderr, "\t%s\n", msg);
    exit(EXIT_FAILURE);
}

void throwUnrecognizedCharError(size_t line, char unrecognized) {
    fprintf(stderr, "[ERROR] Encountered unused/unrecognized character: %c, on line: %zu\n", unrecognized, line);

#ifdef  DEBUG_
    fprintf(stderr, "dec char val: %d", (short)unrecognized);
#endif

    exit(EXIT_FAILURE);
}
