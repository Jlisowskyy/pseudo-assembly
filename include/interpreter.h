//
// Created by Jlisowskyy on 10/7/23.
//

#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <glib.h>

#include "token.h"
#include "list.h"

#define DECL_INSTRUCTION_FOUND 1
#define NON_DECL_INSTRUCTION_FOUND 0

// main machine loops
void interpFileTokens(list_t tokens);
void interpCLTokens(list_t tokens);
void getAllLabels(list_t tokens);

// processing function
inline int isDeclInstruction(const char *ident);
void * processLabel();
inline void processIdent();
inline void addToDataTable(char *ident, void *valPtr);
inline void addToCodeTable(char *ident, node_t *p);
inline node_t* getCodeLabel(char *ptr);
inline MACHINE_BASIC_INT_TYPE getDataVal(char *ptr);
inline char * getDataPtr(char* ptr);

// syntax unpacking

typedef struct RegReg{
    MACHINE_BASIC_INT_TYPE reg1;
    MACHINE_BASIC_INT_TYPE reg2;
}RegReg;

typedef struct RegAddress{
    MACHINE_BASIC_INT_TYPE reg;
    MACHINE_BASIC_INT_TYPE address;
}RegAddress;

typedef struct RegIdent{
    MACHINE_BASIC_INT_TYPE reg;
    char* ident;
} RegIdent;

inline RegReg expectRegReg();
inline RegIdent expectRegIdent();
inline MACHINE_BASIC_INT_TYPE expectReg();
inline char* expectIdent();
inline RegAddress expectRegAddress();

inline void expectEOL();
inline void expectSep();
inline MACHINE_BASIC_INT_TYPE expectRegWoutEOL(const char *errMsg);
inline char* expectIdentWoutEOL(const char* errMsg);
// instruction processors
inline void processADD();
inline void processADD_REG();
inline void processSUB();
inline void processSUB_REG();
inline void processMULT();
inline void processMULT_REG();
inline void processDIV();
inline void processDIV_REG();
inline void processCOMP();
inline void processCOMP_REG();
inline void processLOAD();
inline void processLOAD_REG();
inline void processSTORE();
inline void processLOAD_ADRESS();
inline void processJUMP();
void processJUMP_COND(int8_t sign);
inline void processJUMP_POS();
inline void processJUMP_NEG();
inline void processJUMP_ZERO();
inline void * processDECL();
inline void * processDEF();
inline void processPRINT();
inline void processBreak();

// helping functions
void printInterpState();
inline void resetInterpState();
inline void moveToNextTkn();
inline void updateSign(MACHINE_BASIC_INT_TYPE x);
inline void abortIfMemOverrun();
inline void chopActNode();
void printHashNodeCodeLabel(gpointer key, gpointer value, gpointer user_data);
void printHashNodeDataLabel(gpointer key, gpointer value, gpointer user_data);
inline uint32_t getCorrectLine(token_t);

#endif //INTERPRETER_INTERPRETER_H