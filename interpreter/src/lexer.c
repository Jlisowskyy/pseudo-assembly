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
#define isBlank(x) (x == ' ' || x =='\t')
#define isNewLine(x) (x == '\n')
#define isNotNewLine(x) (x != '\n')
#define isOperSep(x) (x == ',')
#define isLabelSep(x) (x == ':')
#define isNumeric(x) (x >= '0' && x <= '9')
//#define isAlph(x) (x >= 'A' && x <= 'z' && (x <= 'Z' || x >= 'a')) // TODO: check
#define isAlph(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
#define isNonAlphIdent(x) (x == '-' || x == '_')
#define isComment(x) (x == '#')
#define isNotEOF(x) (x != EOF)
#define isNotTerm(x) (x != '\0');
#define isIdentifier(x) (isAlph(x) || isNonAlphIdent(x))

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
uint32_t line = 1;
list_t tokenOutput = { .tail = NULL, .head = NULL };

// -----------------------------------------------
// lexer character operations implementation
// -----------------------------------------------

void resetLexerMachineStates() {
    list_t empty_list = { .head = NULL, .tail = NULL };

    inputPos = 0;
    line = 1;
    tokenSource = NULL;
    cleanList(tokenOutput);
    tokenOutput = empty_list;
}

void processBlank() {
    tokenSource[inputPos] = '\0';
    ++inputPos;
}

void processNewLine() {
    token_t lineSep = { .line = line, .type = LINE_SEP, .strVal = NULL, .numVal = 0 };

    tokenSource[inputPos] = '\0';
    ++inputPos;
    ++line;
    tokenOutput = pushBack(tokenOutput, lineSep);
}

void processLabelSep() {
    tokenSource[inputPos] = '\0';
    ++inputPos;

    if (tokenOutput.tail->tkn.type != IDENTIFIER){
        throwError("Unexpected use of label signing operator (':')", line);
    }

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

    tokenOutput.tail->tkn.type = REGISTER;
}

void processNumeric() {
    token_t constToken = { .type = CONSTANT, .line = line, .strVal = NULL, .numVal = 0 };
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

    constToken.numVal = inputVal;
    tokenOutput = pushBack(tokenOutput, constToken);
}

void processIdentifier() {
    token_t tkn = { .type = IDENTIFIER, .line = line, .strVal = tokenSource + inputPos, .numVal = 0 };

    while(isIdentifier(tokenSource[inputPos])){
        inputPos++;
    }
    tokenOutput = pushBack(tokenOutput, tkn);
}

void processComment() {
    while(isNotNewLine(tokenSource[inputPos]) && isNotEOF(tokenSource[inputPos])){
        tokenSource[inputPos++] = '\0';
    }
}

// ------------------------------------------
// lexer main functions implementation
// ------------------------------------------

list_t convertPlainTextToTokens(char *plainInput) {
    token_t initLabel = { .strVal = BEGIN_LABEL, .type = LABEL, .line = 1, .numVal = 0 };
    tokenOutput = initList(initLabel);
    tokenSource = plainInput;

    while(tokenSource[inputPos] != EOF){
        if (isBlank(tokenSource[inputPos])){
            processBlank();
        }
        else if(isNewLine(tokenSource[inputPos])){
            processNewLine();
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
        else if(isNonAlphIdent(tokenSource[inputPos])){
            processIdentifier();
        }
        else if(isLabelSep(tokenSource[inputPos])){
            processLabelSep();
        }
        else if (isComment(tokenSource[inputPos])){
            processComment();
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
