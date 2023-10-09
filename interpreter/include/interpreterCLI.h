//
// Created by Jlisowskyy on 10/7/23.
//

#ifndef INTERPRETER_INTERPRETERCLI_H
#define INTERPRETER_INTERPRETERCLI_H

#include <stdio.h>
#include "list.h"

#define CLI_BUFFER_CHAR_COUNT 4096
#define CLI_BUFFER_SIZE 4098  //4kb for characters 2bytes for '\n' and '\0'
#define UNRECOGNIZED_FILE 1
#define NO_ARGUMENTS 1

enum inputCommands { NO_COMMANDS = 1, COMMAND_PROCESSED = 2, ADD_LINE = 'a', PRINT = 'p', REMOVE = 'r', EXIT = 'q' };

void interpreterMain(int argc, const char** argv);
int interpreterFile(const char* fileName);
void interpreterInteractive();
void interpret(char *unprocessedContent);

inline int checkForCommandInput(char *buff, list_t tkns);
inline void printWrittenCode(list_t tkns);
inline void addLineToCode(list_t tkns);
inline void removeLineFromCode(list_t tkns);

#endif //INTERPRETER_INTERPRETERCLI_H
