//
// Created by Jlisowskyy on 10/8/23.
//

#include <glib-2.0/glib.h>

#include "../include/errors.h"
#include "../include/interpreter.h"
#include "../include/compMacros.h"

#define HASH(const_ptr) const_ptr[0] + 100 * const_ptr[1]
#define GET_HASH2(a, b) a + 100 * b

#define isNotRegister(token) token.type != CONSTANT || token.numVal < 0 || token.numVal >= MACHINE_REGISTERS

enum {
    ADD_HASH = 'A',
    COMP_HASH = 'C',
    DIV_HASH = 'D',
    JUMP_HASH = 'J',
    LOAD_HASH = 'L',
    MULT_HASH = 'M',
    SUB_HASH = 'S',
    LA_HASH = 6576,
    DC_HASH = 6768,
    JN_HASH = 7874,
    JP_HASH = 8074,
    AR_HASH = 8265,
    CR_HASH = 8267,
    DR_HASH = 8268,
    LR_HASH = 8276,
    MR_HASH = 8277,
    PR_HASH = 8280,
    SR_HASH = 8283,
    DS_HASH = 8368,
    ST_HASH = 8483,
    JZ_HASH = 9074,
};

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
    resetInterpState();

    addToCodeTable(tokens.head);
    actToken = tokens.head->next;

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

void processLabel()
    // used to process language construction

    // TODO: remove added labels
{
    node_t* labelToken;

    if (!actToken->next) {
        addToCodeTable(actToken);
        return;
    }

    labelToken = actToken;
    actToken = actToken->next;

    if (actToken->tkn.type == IDENTIFIER){
        if (isDeclInstruction(actToken->tkn.strVal)){
//            MACHINE_BASIC_TYPE
            // TODO: process decl!!!! ULTIMATE IMPORT NOT WORKING WOUT THIS
            addToDataTable(actToken->tkn, NULL);
        }
        else{
            addToCodeTable(labelToken);
        }
    }
    else if (actToken->tkn.type == LABEL){
        addToCodeTable(labelToken);
    }
    else{
        throwError("Expects instruction after label usage", actToken->tkn.line);
    }
}

void addToDataTable(token_t labelTkn, MACHINE_BASIC_TYPE *val) {
    if (g_hash_table_contains(dataLabelsTable, labelTkn.strVal))
        throwError("Redefinition of data label", labelTkn.line);

    g_hash_table_insert(dataLabelsTable, labelTkn.strVal, val);
}

void addToCodeTable(node_t *p) {
    if (g_hash_table_contains(codeLabelsTable, p->tkn.strVal))
        throwError("Redefinition of code label", p->tkn.line);

    g_hash_table_insert(codeLabelsTable, p->tkn.strVal, p);
}

node_t *getCodeLabel(node_t *p) {
    void* retVal = g_hash_table_lookup(codeLabelsTable, p->tkn.strVal);

    if (!retVal)
        throwError("undefined code label", p->tkn.line);

    return retVal;
}

MACHINE_BASIC_TYPE getDataVal(node_t *p) {
    void* retVal = g_hash_table_lookup(dataLabelsTable, p->tkn.strVal);

    if (!retVal)
        throwError("undefined code label", p->tkn.line);

    return *((MACHINE_BASIC_TYPE*)retVal);
}


int isDeclInstruction(const char *ident)
// Checks if passed identifier contains some decl function in fast way
{
    // safe: array contains addition 3 null bytes
    if (ident[0] != 'D') return NON_DECL_INSTRUCTION_FOUND;
    if ((ident[1] == 'C' || ident[1] == 'S') && ident[2] == '\n') return DECL_INSTRUCTION_FOUND;
    return NON_DECL_INSTRUCTION_FOUND;
}

void processIdent() {
    char* str = actToken->tkn.strVal;

    if (str[2] == '\0' && str[1] == '\0') // safe: array contains addition 3 null bytes
        throwError("Expects instruction on beginning of the line", actToken->tkn.line);

    actToken = actToken->next;

    switch (HASH(str)) {
        case ADD_HASH:
            processADD();
            break;
        case COMP_HASH:
            processCOMP();
            break;
        case DIV_HASH:
            processDIV();
            break;
        case JUMP_HASH:
            processJUMP();
            break;
        case LOAD_HASH:
            processLOAD();
            break;
        case MULT_HASH:
            processMULT();
            break;
        case SUB_HASH:
            processSUB();
            break;
        case LA_HASH:
            processLOAD_ADRESS();
            break;
        case DC_HASH:
            processDECL();
            break;
        case JN_HASH:
            processJUMP_NEG();
            break;
        case JP_HASH:
            processJUMP_POS();
            break;
        case AR_HASH:
            processADD_REG();
            break;
        case CR_HASH:
            processCOMP_REG();
            break;
        case DR_HASH:
            processDIV_REG();
            break;
        case LR_HASH:
            processLOAD_REG();
            break;
        case MR_HASH:
            processMULT_REG();
            break;
        case PR_HASH:
            processPRINT();
            break;
        case SR_HASH:
            processSUB_REG();
            break;
        case DS_HASH:
            processDEF();
            break;
        case ST_HASH:
            processSTORE();
            break;
        case JZ_HASH:
            processJUMP_ZERO();
            break;
        default:
            throwError("Unrecognized instruction", actToken->tkn.line);
    }
}

// ------------------------------------
// Single token expectators procs
// ------------------------------------

MACHINE_BASIC_TYPE expectRegWoutEOL(const char *errMsg) {
    MACHINE_BASIC_TYPE result;
    if (isNotRegister(actToken->tkn)){
        throwError(errMsg, actToken->tkn.line);
    }

    result = actToken->tkn.numVal;
    actToken = actToken->next;

    return result;
}

void expectEOL() {
    if (actToken->tkn.type != LINE_SEP)
        throwError("Instructions have to be separated by lines", actToken->tkn.line);

    actToken = actToken->next;
}

void expectSep() {
    if (actToken->tkn.type != OPER_SEP)
        throwError("Instruction expects comma after first operand", actToken->tkn.line);

    actToken = actToken->next;
}

char *expectIdentWoutEOL(const char *errMsg) {
    char* result;

    if (actToken->tkn.type != IDENTIFIER)
        throwError(errMsg, actToken->tkn.line);

    result = actToken->tkn.strVal;
    actToken = actToken->next;

    return result;
}



// ---------------------------------------------
// syntax checking function implementation
// ---------------------------------------------


RegReg expectRegReg() {
    RegReg result;

    result.reg1 = expectRegWoutEOL("Instruction expects register as first operand");
    expectSep();
    result.reg2 = expectRegWoutEOL("Instruction expects register as second operand");
    expectEOL();

    return result;
}

RegIdent expectRegIdent() {
    RegIdent result;

    result.reg = expectRegWoutEOL("Instruction expects register as first operand");
    expectSep();
    result.ident = expectIdentWoutEOL("Instruction expects register as second operand");
    expectEOL();

    return result;
}

MACHINE_BASIC_TYPE expectReg() {
    MACHINE_BASIC_TYPE result;

    result = expectRegWoutEOL("Instruction takes as argument only single register");
    expectEOL();
    return result;
}

char *expectIdent() {
    char* result;

    expectIdentWoutEOL("Instruction takes as argument only single register");
    expectEOL();
}

void expectDecl() {

}

// --------------------------------------------
// Instruction descriptors implementation
// --------------------------------------------

void processADD() {
    RegIdent args = expectRegIdent();
}

void processADD_REG() {
    RegReg args = expectRegReg();
}

void processSUB() {
    RegIdent args = expectRegIdent();
}

void processMULT() {
    RegIdent args = expectRegIdent();
}

void processMULT_REG() {
    RegReg args = expectRegReg();
}

void processDIV() {
    RegIdent args = expectRegIdent();
}

void processDIV_REG() {
    RegReg args = expectRegReg();
}

void processCOMP() {
    RegIdent args = expectRegIdent();
}

void processCOMP_REG() {
    RegReg args = expectRegReg();
}

void processLOAD() {

}

void processLOAD_REG() {
    char* ident = expectIdent();
}

void processSTORE() {
    char* ident = expectIdent();
}

void processLOAD_ADRESS() {
    char* ident = expectIdent();
}

void processJUMP() {
    char* ident = expectIdent();
}

void processJUMP_POS() {
    char* ident = expectIdent();

}

void processJUMP_NEG() {
    char* ident = expectIdent();
}

void processJUMP_ZERO() {
    char* ident = expectIdent();
}

void processDECL() {

}

void processDEF() {

}

void processPRINT() {
    MACHINE_BASIC_TYPE arg = expectReg();

}

void processSUB_REG() {
    RegReg args = expectRegReg();
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