//
// Created by Jlisowskyy on 10/8/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include "../include/lexer.h"
#include "../include/errors.h"

// ------------------------------
// macros and helpers
// ------------------------------

#define FALSE 0
#define TRUE 1
#define READ_ADDRESS_FROM "O"
#define DECL_MET_BEFORE 1
#define isBlank(x) (x == ' ' || x =='\t')
#define isNewLine(x) (x == '\n')
#define isNotNewLine(x) (x != '\n')
#define isOperSep(x) (x == ',')
#define isLabelSep(x) (x == ':')
#define isNumeric(x) ((x >= '0' && x <= '9') || x == '-')
//#define isAlph(x) (x >= 'A' && x <= 'z' && (x <= 'Z' || x >= 'a')) // TODO: check
#define isAlph(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
#define isNonAlphIdent(x) (x == '-' || x == '_')
#define isComment(x) (x == '#')
#define isNotEOF(x) (x != EOF)
#define isNotTerm(x) (x != '\0')
#define isIdentifier(x) (isAlph(x) || isNonAlphIdent(x))
#define isArrOper(x) (x == '*')
#define isDeclEnd(x) (x == ')')
#define isDeclBeg(x) (x == '(')

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
    token_t sep = { .type = OPER_SEP, .line = line, .numVal = 0, .strVal = NULL };

    // TODO: check for correct error reporting on ,
    if (tokenOutput.tail->tkn.type != CONSTANT && tokenOutput.tail->tkn.type != IDENTIFIER){
        throwError("unexpected use of operand separator (',')", line);
    }

    tokenSource[inputPos] = '\0';
    ++inputPos;

    tokenOutput = pushBack(tokenOutput, sep);
}

void processNumeric()
    // converts character input to actual numeric values and adds to tokens
    // throws when:
    // - number is too big
    // - number is invalid
    // - generally all errors returned by strtol
{
    token_t constToken = { .type = CONSTANT, .line = line, .strVal = NULL, .numVal = 0 };
    MACHINE_BASIC_INT_TYPE inputVal;
    char * resPtr;
    char * begin = tokenSource + inputPos;

    inputVal = MACHINE_STR_TO_NUM_FUNC(begin, &resPtr, 10);

    if (errno == ERANGE){
        throwError("Passed number exceeds machine basic type max values", line);
    }

    if (resPtr == tokenSource){
        throwError("Error occurred during numeric conversion of input", line);
    }

    inputPos += resPtr - begin;
    // TODO: check for space after? expects space after?

    constToken.numVal = inputVal;
    tokenOutput = pushBack(tokenOutput, constToken);
}

void processIdentifier(){
    token_t tkn = { .type = IDENTIFIER, .line = line, .strVal = tokenSource + inputPos, .numVal = 0 };
    // TODO: check for nums in labels

    while(isIdentifier(tokenSource[inputPos])){
        tokenSource[inputPos] = (char)toupper(tokenSource[inputPos]);
        inputPos++;
    }
    tokenOutput = pushBack(tokenOutput, tkn);
}

void processComment() {
    while(isNotNewLine(tokenSource[inputPos])){
        tokenSource[inputPos++] = '\0';
    }
}

void processArray() {
    tokenSource[inputPos] = '\0';
    ++inputPos;

    if (tokenOutput.tail->tkn.type != CONSTANT){
        throwError("expect number aka size of array before declaration", tokenOutput.tail->tkn.line);
    }

    tokenOutput.tail->tkn.type = ARR_SIZE;
}

void processDeclBeg() {
    tokenSource[inputPos] = '\0';
    ++inputPos;

    if (tokenOutput.tail->tkn.type != IDENTIFIER){
        throwError("\"(\" - definition operator expects identifier before", tokenOutput.tail->tkn.line);
    }

    if (strcmp(tokenOutput.tail->tkn.strVal, machineDataTypesIdent[INTEGER]) == 0){
        if (tokenOutput.tail->tkn.numVal == DECL_MET_BEFORE){
            throwError("only single use of \"(\" allowed", tokenOutput.tail->tkn.line);
        }

        tokenOutput.tail->tkn.type = INTEGER_TYPE;
        tokenOutput.tail->tkn.numVal = DECL_MET_BEFORE;
    }
    else if (strcmp(tokenOutput.tail->tkn.strVal, READ_ADDRESS_FROM) == 0){
        tokenOutput.tail->tkn.type = ADDRESS_READ;
    }
    else{
        throwError("Not allowed usage of parenthesis: expected only in data definition and o(reg) construction", tokenOutput.tail->tkn.line);
    }

    size_t my_pos = inputPos;
    while(isNotNewLine(tokenSource[++my_pos])){
        if (isDeclEnd(tokenSource[my_pos])){
            tokenSource[my_pos] = ' ';
            return;
        }
    }

    throwError("Parenthesis is not closed", tokenOutput.tail->tkn.line);
}

// ------------------------------------------
// lexer main functions implementation
// ------------------------------------------

list_t convertPlainTextToTokens(char *plainInput) {
    token_t newLineSentinel = { .type = LINE_SEP, .numVal = 0, .strVal = NULL, .line = 1 };
    tokenOutput = initList(newLineSentinel);
    tokenSource = plainInput;

    while(isNotTerm(tokenSource[inputPos])){
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
        else if(isAlph(tokenSource[inputPos]) || isNonAlphIdent(tokenSource[inputPos])){
            processIdentifier();
        }
        else if(isLabelSep(tokenSource[inputPos])){
            processLabelSep();
        }
        else if (isComment(tokenSource[inputPos])){
            processComment();
        }
        else if (isArrOper(tokenSource[inputPos])){
            processArray();
        }
        else if(isDeclBeg(tokenSource[inputPos])){
            processDeclBeg();
        }
        else{
            throwUnrecognizedCharError(line, tokenSource[inputPos]);
        }
    }

    newLineSentinel.line = line;
    tokenOutput = pushBack(tokenOutput, newLineSentinel);
    return tokenOutput;
}

list_t convertCLInputToTokens(list_t tokenStream, char *CLInput) {
    list_t result;
    return result;
}
