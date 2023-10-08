//
// Created by Jlisowskyy on 10/7/23.
//

#ifndef INTERPRETER_INTERPRETERCLI_H
#define INTERPRETER_INTERPRETERCLI_H

#include <stdio.h>

#define CLI_BUFFER_CHAR_COUNT 4096
#define CLI_BUFFER_SIZE 4098  //4kb for characters 2bytes for '\n' and '\0'
#define UNRECOGNIZED_FILE 1
#define NO_ARGUMENTS 1
#define CLI_EXIT "_exit_\n"

#define DEBUG_

void interpreterMain(int argc, const char** argv);
int interpreterFile(const char* fileName);
void interpreterInteractive();
void interpret(char *unprocessedContent);

#endif //INTERPRETER_INTERPRETERCLI_H
