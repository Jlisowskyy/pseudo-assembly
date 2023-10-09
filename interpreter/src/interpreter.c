//
// Created by Jlisowskyy on 10/8/23.
//

#include <glib-2.0/glib.h>

#include "../include/errors.h"
#include "../include/interpreter.h"
#include "../include/compMacros.h"

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

// ---------------------------------------
// global interpreter machine states
// ---------------------------------------

int isNewInstructionLine = TRUE;
node_t* actToken = NULL;

MACHINE_BASIC_TYPE registers[MACHINE_REGISTERS] = { 0 };
GHashTable* codeLabelsTable = NULL;
GHashTable* dataLabelsTable = NULL;

// ----------------------------------------
// interpreter machine implementation
// ----------------------------------------

void interpFileTokens(list_t tokens) {
    actToken = tokens.head->next;
    // TODO add head to code labels

    while(actToken){
        switch (actToken->tkn.type) {

            case IDENTIFIER:
                break;
            case LABEL:
                processLabel();
                break;
            case LINE_SEP:
                isNewInstructionLine = TRUE;
                break;
            default:
                throwError("Unkown token type occurred\n", actToken->tkn.line);
        }

        actToken = actToken->next;
    }
}

void interpCLTokens(list_t tokens) {

}

// ----------------------------------------
// processing functions implementation
// ----------------------------------------

void processLabel() {
    if (!actToken->next) return;

    if (isDeclInstruction(actToken->next->tkn.strVal)){
        addToDataTable(actToken);
    }
    else{
        addToCodeTable(actToken);
    }

}

void addToDataTable(node_t *p) {

}

void addToCodeTable(node_t *p) {

}

int isDeclInstruction(const char *ident)
// Checks if passed identifier contains some decl function in fast way
// TODO: check for index overcome
{
    if (ident[0] != 'D') return NON_DECL_INSTRUCTION_FOUND;
    if ((ident[1] == 'C' || ident[1] == 'S') && ident[2] == '\n') return DECL_INSTRUCTION_FOUND;
    return NON_DECL_INSTRUCTION_FOUND;
}

void processIdent() {

}

// ---------------------------------------------------
// Default settings - only working implementation
// ---------------------------------------------------

void printInterpState() {

}