//
// Created by Jlisowskyy on 10/8/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "../include/lexer.h"

// ------------------------------
// macros and helpers
// ------------------------------


#define FALSE 0
#define TRUE 1
#define isSpace(x) x == ' '
#define isNewLine(x) x == '\n'
#define isOperSep(x) x == ','
#define isLabelSep(x) x == ':'



// ------------------------------
// char consts
// ------------------------------

const char* instructionAliases[INSTRUCTION_COUNT] = {
        "A",
        "AR",
        "S",
        "SR",
        "M",
        "MR",
        "D",
        "DR",
        "C",
        "CR",
        "L",
        "LR",
        "ST",
        "LA",
        "J",
        "JP",
        "JN",
        "JP",
        "DC",
        "DS",
        "PR",
};

const char* BEGIN_LABEL = "_BEGIN:\n";

// ------------------------------
// lexer machine states
// ------------------------------

char* tokenSource = NULL;
size_t input_pos = 0;
int isNewToken = TRUE;
uint32_t line = 1;
list_t tokenOutput = { .tail = NULL, .head = NULL };

// -----------------------------------------------
// lexer character operations implementation
// -----------------------------------------------

void resetLexerMachineStates() {
    list_t empty_list = { .head = NULL, .tail = NULL };

    input_pos = 0;
    isNewToken = TRUE;
    line = 1;
    tokenSource = NULL;
    cleanList(tokenOutput);
    tokenOutput = empty_list;
}

void processSpace() {
    tokenSource[input_pos] = '\0';
    ++input_pos;
    isNewToken = TRUE;
}

void processNewLine() {
    token_t lineSep = { .line = line, .type = LINE_SEP };

    tokenSource[input_pos] = '\0';
    ++input_pos;
    ++line;
    isNewToken = TRUE;
    tokenOutput = pushBack(tokenOutput, lineSep);
}

void processLabelSep() {

}

void processOperSep() {

}


// ------------------------------------------
// lexer main functions implementation
// ------------------------------------------

list_t convertPlainTextToTokens(char *plainInput) {
    token_t initLabel = { .identifier = BEGIN_LABEL };
    tokenOutput = initList(initLabel);
    tokenSource = plainInput;

    while(tokenSource[input_pos] != EOF){
        if (isSpace(tokenSource[input_pos])){
            processSpace();
        }
        else if(isNewLine(tokenSource[input_pos])){
            processNewLine();
        }
        else if(isLabelSep(tokenSource[input_pos])){

        }
    }

    return tokenOutput;
}

list_t convertCLInputToTokens(list_t tokenStream, char *CLInput) {
    list_t result;
    return result;
}
