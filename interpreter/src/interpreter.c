//
// Created by Jlisowskyy on 10/8/23.
//

#include <glib-2.0/glib.h>

#include "../include/errors.h"
#include "../include/interpreter.h"
#include "../include/compMacros.h"


// ---------------------------------------
// global interpreter machine states
// ---------------------------------------

node_t* actToken = NULL;

MACHINE_BASIC_TYPE registers[MACHINE_REGISTERS] = { 0 };
GHashTable* codeLabelsTable = NULL;
GHashTable* dataLabelsTable = NULL;

// ----------------------------------------
// interpreter machine implementation
// ----------------------------------------

void interpFileTokens(list_t tokens) {
    actToken = tokens.head->next;
    resetInterpState();
    // TODO add head to code labels

    while(actToken){
        switch (actToken->tkn.type) {

            case IDENTIFIER:
                processIdent();
                break;
            case LABEL:
                processLabel();
                break;
            case LINE_SEP:
                break;
            default:
                throwError("Unknown token type occurred\n", actToken->tkn.line);
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

node_t *getCodeLabel(const char *ident) {
    return NULL;
}

MACHINE_BASIC_TYPE getDataVal(const char *ident) {
    return 0;
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

// --------------------------------------------
// Instruction descriptors implementation
// --------------------------------------------

void processADD() {

}

void processADD_REG() {

}

void processSUB() {

}

void processMULT() {

}

void processMULT_REG() {

}

void processDIV() {

}

void processDIV_REG() {

}

void processCOMP() {

}

void processCOMP_REG() {

}

void processLOAD() {

}

void processLOAD_REG() {

}

void processSTORE() {

}

void processLOAD_ADRESS() {

}

void processJUMP() {

}

void processJUMP_POS() {

}

void processJUMP_NEG() {

}

void processJUMP_ZERO() {

}

void processDECL() {

}

void processDEF() {

}

void processPRINT() {

}

// ---------------------------------------------------
// Default settings - only working implementation
// ---------------------------------------------------

void printInterpState() {

}

void resetInterpState() {
    if (codeLabelsTable) g_hash_table_remove_all(codeLabelsTable);
    else codeLabelsTable = g_hash_table_new(g_str_hash, g_str_equal);

    if (dataLabelsTable) g_hash_table_remove_all(dataLabelsTable);
    else dataLabelsTable = g_hash_table_new(g_str_hash, g_str_equal);
}
