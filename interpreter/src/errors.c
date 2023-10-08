//
// Created by Jlisowskyy on 10/7/23.
//
#include "../include/errors.h"
#include <stdio.h>

void writeHelpOut(char *unrecognizedFile) {
    fprintf(stderr, "[ERROR] Unrecognized file: %s. Expected usage:\n\t\"pas-int FILE_NAME\" to interpret from file\n", unrecognizedFile);
    fprintf(stderr, "\t\"pas-int\" to run interactive mode with all resources on display\n");
}

void throwError(const char *msg, size_t line) {
    fprintf(stderr, "[ERROR] Encountered error on line: %zu\n", line);
    fprintf(stderr, "\t%s\n", msg);
    exit(EXIT_FAILURE);
}

void throwUnrecognizedCharError(size_t line, char unrecognized) {
    fprintf(stderr, "[ERROR] Encountered unused/unrecognized character: %c, on line: %zu", unrecognized, line);
    exit(EXIT_FAILURE);
}
