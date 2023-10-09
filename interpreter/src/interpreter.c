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

// ----------------------------------------
// interpreter machine implementation
// ----------------------------------------

void interpFileTokens(list_t tokens) {
    actToken = tokens.head;

    while(actToken){
        switch (actToken->tkn.type) {

            case UNKNOWN:
                throwError("Unkown token type occurred\n", actToken->tkn.line);
            case IDENTIFIER:
                break;
            case REGISTER:
                break;
            case LABEL:
                break;
            case CONSTANT:
                break;
            case LINE_SEP:
                break;
        }

        actToken = actToken->next;
    }
}

void interpCLTokens(list_t tokens) {

}

void printInterpState() {

}
