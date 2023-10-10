//
// Created by Jlisowskyy on 10/7/23.
//

#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include "token.h"
#include "list.h"

#define DECL_INSTRUCTION_FOUND 1
#define NON_DECL_INSTRUCTION_FOUND 0

// main machine loops
void interpFileTokens(list_t tokens);
void interpCLTokens(list_t tokens);

// processing function
inline int isDeclInstruction(const char *ident);
inline void processLabel();
inline void processIdent();
inline void addToDataTable(node_t* p);
inline void addToCodeTable(node_t* p);
inline node_t* getCodeLabel(const char* ident);
inline MACHINE_BASIC_TYPE getDataVal(const char* ident);

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
        "JZ",
        "DC",
        "DS",
        "PR",
};

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
inline void processJUMP_POS();
inline void processJUMP_NEG();
inline void processJUMP_ZERO();
inline void processDECL();
inline void processDEF();
inline void processPRINT();

// helping functions
void printInterpState();
void resetInterpState();

#endif //INTERPRETER_INTERPRETER_H

