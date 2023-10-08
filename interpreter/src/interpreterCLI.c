//
// Created by Jlisowskyy on 10/7/23.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/interpreterCLI.h"
#include "../include/errors.h"
#include "../include/list.h"
#include "../include/lexer.h"
#include "../include/interpreter.h"

void interpreterMain(int argc, const char **argv)
    // entry point to the program, invokes correct behaving interaface
{
    int result;

    if (argc == NO_ARGUMENTS){
        interpreterInteractive();
        return;
    }

    for(size_t i = 0; i < argc; ++i){
        result = interpreterFile(argv[1]);
        if (result == UNRECOGNIZED_FILE){
            writeHelpOut(NULL);
            exit(EXIT_FAILURE);
        }
    }
}

int interpreterFile(const char *fileName) {
    FILE* workFile;
    size_t fileSize;
    char* plainInput;

    workFile = fopen(fileName, "r");
    if (!workFile) return UNRECOGNIZED_FILE;

    fseek(workFile, 0, SEEK_END);
    fileSize = ftell(workFile) + 1;
    rewind(workFile);

    plainInput = malloc(fileSize);
    fread(plainInput, sizeof(char), fileSize, workFile);
    plainInput[fileSize] = '\0';
    fclose(workFile);

    interpret(plainInput);
    free(plainInput);
}

void interpreterInteractive() {
    char buffer[CLI_BUFFER_SIZE];
    list_t tokens = { .head = NULL, .tail = NULL };

    printf("Welcome to interactive mode! To exit type: \"_exit_\"\nPlease enter your instructions lines separated by semi-colons:\n");
    fgets(buffer, CLI_BUFFER_CHAR_COUNT, stdin);

    while(strcmp(buffer, CLI_EXIT) != 0){
        printf("test: %s\n", buffer);
        tokens = convertCLInputToTokens(tokens, buffer);
        interpCLTokens(tokens);
        printInterpState();
        fgets(buffer, CLI_BUFFER_CHAR_COUNT, stdin);
    }

    printf("Terminated successfully!\n");
}

void interpret(char *unprocessedContent) {
    list_t tokens;

    tokens = convertPlainTextToTokens(unprocessedContent);

#ifdef DEBUG_
    printTknList(tokens);
#endif

    interpFileTokens(tokens);
}
