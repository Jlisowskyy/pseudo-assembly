//
// Created by Jlisowskyy on 10/7/23.
//
#include "../include/errors.h"
#include <stdio.h>

void writeHelpOut(char *unrecognizedFile) {
    fprintf(stderr, "[ERROR] Unrecognized file: %s. Expected usage:\n\t\"pas-int FILE_NAME\" to interpret from file\n", unrecognizedFile);
    fprintf(stderr, "\t\"pas-int\" to run interactive mode with all resources on display\n");
}