//
// Created by Jlisowskyy on 10/8/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "../include/lexer.h"
#include "../include/errors.h"

// ------------------------------
// macros and helpers
// ------------------------------

#define FALSE 0
#define TRUE 1
#define isSpace(x) x == ' '
#define isNewLine(x) x == '\n'
#define isOperSep(x) x == ','
#define isLabelSep(x) x == ':'
#define isNumeric(x) x >= '1' && x <= '9'
#define isAlph(x) x >= 'A' && x <= 'z' && (x <= 'Z' || x >= 'a') // TODO: check

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

const char* BEGIN_LABEL = "_BEGIN";

// ------------------------------
// lexer machine states
// ------------------------------

char* tokenSource = NULL;
size_t inputPos = 0;
int isNewToken = TRUE;
uint32_t line = 1;
list_t tokenOutput = { .tail = NULL, .head = NULL };

// -----------------------------------------------
// lexer character operations implementation
// -----------------------------------------------

void resetLexerMachineStates() {
    list_t empty_list = { .head = NULL, .tail = NULL };

    inputPos = 0;
    isNewToken = TRUE;
    line = 1;
    tokenSource = NULL;
    cleanList(tokenOutput);
    tokenOutput = empty_list;
}

void processSpace() {
    tokenSource[inputPos] = '\0';
    ++inputPos;
    isNewToken = TRUE;
}

void processNewLine() {
    token_t lineSep = { .line = line, .type = LINE_SEP, .identifier = NULL, .val = 0};

    tokenSource[inputPos] = '\0';
    ++inputPos;
    ++line;
    isNewToken = TRUE;
    tokenOutput = pushBack(tokenOutput, lineSep);
}

void processLabelSep() {
    tokenSource[inputPos] = '\0';
    ++inputPos;
    isNewToken = TRUE;

    tokenOutput.tail->tkn.type = LABEL;
}

void processOperSep()
    // operand separator expects, that previous token is const value within range 0-(register count - 1),
    // other input is nothing than error
{
    if (tokenOutput.tail->tkn.type != CONSTANT){
        throwError("unexpected use of operand separator (',')", line);
    }

    tokenSource[inputPos] = '\0';
    ++inputPos;
    isNewToken = TRUE;

    tokenOutput.tail->tkn.type = REGISTER;
}

void processNumeric() {
    token_t constToken = { .type = CONSTANT, .line = line, .identifier = NULL };
    long inputVal;
    char * resPtr;
    char * begin = tokenSource + inputPos;
    size_t offset;

    inputVal = strtol(begin, &resPtr, 10);

    if (resPtr == tokenSource){
        throwError("Error occurred during numeric conversion of input", line);
    }

    inputPos += resPtr - begin;
    // TODO: check for space after? expects space after?

    constToken.val = inputVal;
    tokenOutput = pushBack(tokenOutput, constToken);
}

void processIdentifier() {

}


// ------------------------------------------
// lexer main functions implementation
// ------------------------------------------

list_t convertPlainTextToTokens(char *plainInput) {
    token_t initLabel = { .identifier = BEGIN_LABEL, .type = LABEL, .line = 1, .val = 0 };
    tokenOutput = initList(initLabel);
    tokenSource = plainInput;

    while(tokenSource[inputPos] != EOF){
        if (isSpace(tokenSource[inputPos])){
            processSpace();
        }
        else if(isNewLine(tokenSource[inputPos])){
            processNewLine();
        }
        else if(isLabelSep(tokenSource[inputPos])){
            processLabelSep();
        }
        else if (isOperSep(tokenSource[inputPos])){
            processOperSep();
        }
        else if (isNumeric(tokenSource[inputPos])){
            processNumeric();
        }
        else if(isAlph(tokenSource[inputPos])){
            processIdentifier();
        }
        else{
            throwUnrecognizedCharError(line, tokenSource[inputPos]);
        }
    }

    return tokenOutput;
}

list_t convertCLInputToTokens(list_t tokenStream, char *CLInput) {
    list_t result;
    return result;
}