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
#include "../include/compMacros.h"

void interpreterMain(int argc, const char **argv)
    // entry point to the program, invokes correct behaving interface
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

int interpreterFile(const char *fileName)
    // opens, loads whole file into memory and finally add necessary modifications
{
    FILE* workFile;
    size_t fileSize;
    char* plainInput;

    workFile = fopen(fileName, "r");
    if (!workFile) return UNRECOGNIZED_FILE;

    fseek(workFile, 0, SEEK_END);
    fileSize = ftell(workFile);
    rewind(workFile);

    plainInput = malloc(fileSize + NULL_BYTES + NEW_LINE_SENTINEL);
    fread(plainInput, sizeof(char), fileSize, workFile);
    plainInput[fileSize] = '\n';
    // new line sentinel to be added to token list, used to prevent checking inside of syntax analyzers

    for(size_t i = 0; i < NULL_BYTES; ++i) plainInput[fileSize + NEW_LINE_SENTINEL + i] = '\0';
    fclose(workFile);

    interpret(plainInput);
    free(plainInput);
    return 0;
}

void interpreterInteractive()
    // reads line of code from stdin and interprets it interactivly
    // TODO
{
    char buffer[CLI_BUFFER_SIZE];
    int inputState;
    list_t tokens = { .head = NULL, .tail = NULL };

    printf("Welcome to interactive mode! To exit type: \"q\"\n");
    printf("To print whole code: \"p\"\nTo remove desired line: \"r\"\n");
    printf("To add line after: \"a\"\n");
    printf("Please enter your instructions lines separated by semi-colons:\n");
    fgets(buffer, CLI_BUFFER_CHAR_COUNT, stdin);

    while(checkForCommandInput(buffer, tokens) != EXIT){
        tokens = convertCLInputToTokens(tokens, buffer);
        interpCLTokens(tokens);
        fgets(buffer, CLI_BUFFER_CHAR_COUNT, stdin);
    }

    printf("Terminated successfully!\n");
}

void interpret(char *unprocessedContent)
    // unprocessedContent - whole file without EOF, with added 2 null bytes '\0' and \n value at the end ( "\0\0\n")
    // \0\0 are guard for identifiers hash checking
    // \n is guard to used to simplify EOF OR EOL just to EOL check
{
    list_t tokens;

    tokens = convertPlainTextToTokens(unprocessedContent);

#ifdef DEBUG_
    printTknList(tokens);
#endif

    interpFileTokens(tokens);
}

// -------------------------------------------
// other helping function implementation
// -------------------------------------------

int checkForCommandInput(char *buff, list_t tkns)
    // dispatching command inside interactive mode
{
    if (buff[1] != '\n') return NO_COMMANDS;

    switch (buff[0]) {
        case EXIT:
            return EXIT;
        case ADD_LINE:
            addLineToCode(tkns);
            return COMMAND_PROCESSED;
        case PRINT:
            printWrittenCode(tkns);
            return COMMAND_PROCESSED;
        case REMOVE:
            removeLineFromCode(tkns);
            return COMMAND_PROCESSED;
        default:
            return NO_COMMANDS;
    }
}

void printWrittenCode(list_t tkns) {

}

void addLineToCode(list_t tkns) {

}

void removeLineFromCode(list_t tkns) {

}
