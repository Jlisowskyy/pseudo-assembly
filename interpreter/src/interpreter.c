//
// Created by Jlisowskyy on 10/8/23.
//

#include <stdio.h>
#include <glib-2.0/glib.h>

#include "../include/errors.h"
#include "../include/interpreter.h"

#define UNKNOWN_SIGN 2
#define POS 1
#define ZERO 0
#define NEG (-1)

#define HASH(const_ptr) (const_ptr[0] + 100 * const_ptr[1])
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
    BREAK = 8266,
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

node_t* pToken = NULL;
node_t* actToken = NULL;

// TODO: place holder: (instruction should change token by themselves - in future)
node_t jump_buffer = { .next = NULL };

int8_t signReg = UNKNOWN_SIGN;
MACHINE_BASIC_INT_TYPE registers[MACHINE_REGISTERS] = {0 };
size_t usedRAMSpace = 0;

GHashTable* codeLabelsTable = NULL;
GHashTable* dataLabelsTable = NULL;

// ----------------------------------------
// interpreter machine implementation
// ----------------------------------------

void getAllLabels(list_t tokens) {
    pToken = tokens.head;
    actToken = tokens.head->next;

    while(actToken){
        if (actToken->tkn.type == LABEL){
            processLabel();
        }
        else moveToNextTkn();
    }
}

void interpFileTokens(list_t tokens) {
    resetInterpState();
    getAllLabels(tokens);

    addToCodeTable("_BEGIN", tokens.head);
    addToCodeTable("_END", tokens.tail);
    actToken = tokens.head->next;

    while(actToken){
        switch (actToken->tkn.type) {

            case IDENTIFIER:
                processIdent();
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

void * processLabel()
    // used to process language construction

    // TODO: remove recursive call
    // TODO: unprotected malloc
{
    void* dataPtr = NULL;
    char* labelIdent = actToken->tkn.strVal;
    chopActNode();

    if (actToken->tkn.type == IDENTIFIER){

        // TODO: speedup
        if (isDeclInstruction(actToken->tkn.strVal)){

            switch (HASH(actToken->tkn.strVal)) {
                case DS_HASH:
                    chopActNode();
                    dataPtr = processDEF();
                    break;
                case DC_HASH:
                    chopActNode();
                    dataPtr = processDECL();
                    break;
            }

            addToDataTable(labelIdent, dataPtr);
        }
        else{
            addToCodeTable(labelIdent, actToken);
        }
    }
    else if (actToken->tkn.type == LABEL){
        if (processLabel())
            throwError("Multiply definition of symbol is not possible", actToken->tkn.line);

        addToCodeTable(labelIdent, actToken);
    }
    else{
        throwError("Expects instruction after label usage", actToken->tkn.line);
    }

    return dataPtr;
}

void addToDataTable(char *ident, void *valPtr) {
    if (g_hash_table_contains(dataLabelsTable, ident))
        throwError("Redefinition of data label", actToken->tkn.line - 1);

    g_hash_table_insert(dataLabelsTable, ident, valPtr);
}

void addToCodeTable(char *ident, node_t *p) {
    if (g_hash_table_contains(codeLabelsTable, ident))
        throwError("Redefinition of code label", p->tkn.line);

    g_hash_table_insert(codeLabelsTable, ident, p);
}

node_t *getCodeLabel(char *ptr) {
    void* retVal = g_hash_table_lookup(codeLabelsTable, ptr);

    if (!retVal)
        throwError("undefined code label", actToken->tkn.line);

    return retVal;
}

MACHINE_BASIC_INT_TYPE getDataVal(char *ptr) {
    void* retVal = g_hash_table_lookup(dataLabelsTable, ptr);

    if (!retVal)
        throwError("undefined code label", actToken->tkn.line - 1);

    return *((MACHINE_BASIC_INT_TYPE*)retVal);
}

char * getDataPtr(char *ptr) {
    void* retVal = g_hash_table_lookup(dataLabelsTable, ptr);

    if (!retVal)
        throwError("undefined code label", actToken->tkn.line - 1);

    return ((char*)retVal);
}

int isDeclInstruction(const char *ident)
// Checks if passed identifier contains some decl function in fast way
{
    // safe: array contains addition 3 null bytes
    if (ident[0] != 'D') return NON_DECL_INSTRUCTION_FOUND;
    if ((ident[1] == 'C' || ident[1] == 'S') && ident[2] == '\0') return DECL_INSTRUCTION_FOUND;
    return NON_DECL_INSTRUCTION_FOUND;
}

void processIdent() {
    char* str = actToken->tkn.strVal;

    if (str[2] != '\0' && str[1] != '\0') // safe: array contains addition 3 null bytes
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
            throwError("Untagged declaration occurred. Use LABEL: before declaration", actToken->tkn.line);
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
        case BREAK:
            processBreak();
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
            throwError("Untagged declaration occurred. Use LABEL: before declaration", actToken->tkn.line);
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

MACHINE_BASIC_INT_TYPE expectRegWoutEOL(const char *errMsg) {
    MACHINE_BASIC_INT_TYPE result;
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
    result.ident = expectIdentWoutEOL("Instruction expects identifier as second operand");
    expectEOL();

    return result;
}

RegAddress expectRegAddress() {
    RegAddress result;

    result.reg = expectRegWoutEOL("Instruction expects register as first operand");
    expectSep();

    if (actToken->tkn.type == ADDRESS_READ){
        actToken = actToken->next;

        if (actToken->tkn.type != CONSTANT){
            throwError("Read address operator ( o(reg) ) - expects register", actToken->tkn.line);
        }

        if (actToken->tkn.numVal < 0 || actToken->tkn.numVal >= MACHINE_REGISTERS){
            throwError("Passed number doesn't match machine registers count", actToken->tkn.line);
        }

        result.address = registers[actToken->tkn.numVal];
        actToken = actToken->next;
    }
    else{
        char* ident = expectIdentWoutEOL("Instruction expects identifier as second operand");
        char* address = getDataPtr(ident);

        result.address = (MACHINE_BASIC_INT_TYPE)address;
    }
    expectEOL();

    return result;
}

MACHINE_BASIC_INT_TYPE expectReg() {
    MACHINE_BASIC_INT_TYPE result;

    result = expectRegWoutEOL("Instruction takes as argument only single register");
    expectEOL();
    return result;
}

char *expectIdent() {
    char* result;

    result = expectIdentWoutEOL("Instruction takes as argument only single register");
    expectEOL();
    return result;
}

// --------------------------------------------
// Instruction descriptors implementation
// --------------------------------------------

void processSUB_REG() {
    RegReg args = expectRegReg();
    registers[args.reg1] -= registers[args.reg2];
    updateSign(registers[args.reg1]);
}

void processADD() {
    RegIdent args = expectRegIdent();
    MACHINE_BASIC_INT_TYPE op2Val = getDataVal(args.ident);

    registers[args.reg] += op2Val;
    updateSign(registers[args.reg]);
}

void processADD_REG() {
    RegReg args = expectRegReg();
    registers[args.reg1] += registers[args.reg2];
    updateSign(registers[args.reg1]);
}

void processSUB() {
    RegIdent args = expectRegIdent();
    MACHINE_BASIC_INT_TYPE op2Val = getDataVal(args.ident);

    registers[args.reg] -= op2Val;
    updateSign(registers[args.reg]);
}

void processMULT() {
    RegIdent args = expectRegIdent();
    MACHINE_BASIC_INT_TYPE op2Val = getDataVal(args.ident);

    registers[args.reg] *= op2Val;
    updateSign(registers[args.reg]);
}

void processMULT_REG() {
    RegReg args = expectRegReg();
    registers[args.reg1] *= registers[args.reg2];
    updateSign(registers[args.reg1]);
}

void processDIV() {
    RegIdent args = expectRegIdent();
    MACHINE_BASIC_INT_TYPE op2Val = getDataVal(args.ident);

    registers[args.reg] /= op2Val;
    updateSign(registers[args.reg]);
}

void processDIV_REG() {
    RegReg args = expectRegReg();
    registers[args.reg1] /= registers[args.reg2];
    updateSign(registers[args.reg1]);
}

void processCOMP() {
    RegIdent args = expectRegIdent();
    MACHINE_BASIC_INT_TYPE op2Val = getDataVal(args.ident);
    updateSign(registers[args.reg] - op2Val);
}

void processCOMP_REG() {
    RegReg args = expectRegReg();
    updateSign(registers[args.reg1] - registers[args.reg2]);
}

void processLOAD() {
    RegAddress args = expectRegAddress();
    registers[args.reg] = *((MACHINE_BASIC_INT_TYPE*)args.address);
}

void processLOAD_REG() {
    RegReg args = expectRegReg();
    registers[args.reg1] = registers[args.reg2];
}

void processSTORE() {
    RegAddress args = expectRegAddress();
    *((MACHINE_BASIC_INT_TYPE*)args.address) = registers[args.reg];
}

// TODO: TOTALLY UNSAFE SHIT XDDDDDDDDDD
void processLOAD_ADRESS() {
    RegIdent args = expectRegIdent();
    registers[args.reg] = (MACHINE_BASIC_INT_TYPE)getDataPtr(args.ident);
}

void processJUMP() {
    char* ident = expectIdent();
    node_t* codePtr = getCodeLabel(ident);

    jump_buffer.next = codePtr;
    actToken = &jump_buffer;
}

// TODO: make single function
void processJUMP_POS() {
    char* ident = expectIdent();

    node_t* codePtr = getCodeLabel(ident);

    if (signReg == POS){
        jump_buffer.next = codePtr;
        actToken = &jump_buffer;
    }
}

void processJUMP_NEG() {
    char* ident = expectIdent();

    node_t* codePtr = getCodeLabel(ident);

    if (signReg == NEG){
        jump_buffer.next = codePtr;
        actToken = &jump_buffer;
    }
}

void processJUMP_ZERO() {
    char* ident = expectIdent();

    node_t* codePtr = getCodeLabel(ident);

    if (signReg == ZERO){
        jump_buffer.next = codePtr;
        actToken = &jump_buffer;
    }
}

// TODO: think through usage of tagged union
void * processDECL()

{
    void* dataPtr = NULL;
    size_t allocSize = 1;
    tokenType_t type;

    if (actToken->tkn.type == ARR_SIZE){
        if (actToken->tkn.numVal < 1){
            throwError("Expected array size is bigger than 0", actToken->tkn.line);
        }

        allocSize = actToken->tkn.numVal;
        chopActNode();
    }

    type = actToken->tkn.type;
    chopActNode();

    if (type == INTEGER_TYPE){
        dataPtr = malloc(sizeof(MACHINE_BASIC_INT_TYPE) * allocSize);
        usedRAMSpace += sizeof(MACHINE_BASIC_INT_TYPE) * allocSize;
        abortIfMemOverrun();

        if (actToken->tkn.type == CONSTANT){

            for (size_t i = 0; i < allocSize; ++i){
                ((MACHINE_BASIC_INT_TYPE*)dataPtr)[i] = actToken->tkn.numVal;
            }
            chopActNode();
        }
        else{
            throwError("Declaration expects to insert data value after data type identifier", actToken->tkn.line);
        }
    }
    else{
        throwError("Unexpected data type occurred", actToken->tkn.line);
    }

    expectEOL();
    chopActNode();
    return dataPtr;
}

void * processDEF() {
    tokenType_t type;
    size_t allocSize = 1;
    char* ident;

    if (actToken->tkn.type == ARR_SIZE){
        if (actToken->tkn.numVal < 1){
            throwError("Expected array size is bigger than 0", actToken->tkn.line);
        }

        allocSize = actToken->tkn.numVal;
        chopActNode();
    }

    if (actToken->tkn.type != IDENTIFIER){
        throwError("expected data type label after declaration", actToken->tkn.line);
    }

    ident = actToken->tkn.strVal;
    chopActNode();
    expectEOL();
    chopActNode();

    if (strcmp(ident, machineDataTypesIdent[INTEGER]) == 0){
        usedRAMSpace += sizeof(MACHINE_BASIC_INT_TYPE) * allocSize;
        abortIfMemOverrun();
        return malloc(sizeof(MACHINE_BASIC_INT_TYPE) * allocSize);
    }
    else{
        throwError("Unrecognized data type name encountered", actToken->tkn.line);
    }

    return NULL; // is not necessary?
}

void processPRINT() {
    MACHINE_BASIC_INT_TYPE arg = expectReg();

    // TODO: WARNING! Should be changed accordingly to added/modified data types
    printf("Register %ld value: %ld\n", arg, registers[arg]);
}

void processBreak() {
    expectEOL();
    printInterpState();

    printf("\n\nPress enter to process till next breakpoint...");
    getchar();
    printf("\n---------------------------------------\n");
}


// ---------------------------------------------------
// Helping functions
// ---------------------------------------------------

// ROWS X COLS should be MACHINE_REGISTERS_COUNT
#define PRINT_REG_ROWS 2
#define PRINT_REG_COLS 8

static const char* signsChars = "-0+";

void printInterpState() {
    printf("Machine state:\nUsed memory: %zu (in Bytes)\nSign reg state: ", usedRAMSpace);

    if (signReg == UNKNOWN_SIGN) printf("register not set");
    else putchar(signsChars[signReg + 1]);
    putchar('\n');

    for(int i = 0; i < PRINT_REG_ROWS; ++i){
        for(int j = 0; j < PRINT_REG_COLS; ++j){
            int regIdent = i * PRINT_REG_COLS + j;
            printf("Reg %d: %ld\t", regIdent, registers[regIdent]);
        }
        putchar('\n');
    }

    printf("\nDefined code labels ( ident ::: line ):\n");
    g_hash_table_foreach(codeLabelsTable, &printHashNodeCodeLabel, NULL);

    printf("\nDefined data identifiers ( ident ::: type/value )\n");
    g_hash_table_foreach(dataLabelsTable, &printHashNodeDataLabel, NULL);
}

void resetInterpState() {
    if (codeLabelsTable) g_hash_table_remove_all(codeLabelsTable);
    else codeLabelsTable = g_hash_table_new(g_str_hash, g_str_equal);

    if (dataLabelsTable) {
        GPtrArray* values = g_hash_table_get_values_as_ptr_array(dataLabelsTable);
        for (int i = 0; i < values->len; ++i) free(values->pdata[i]);
        g_free(values);
        g_hash_table_remove_all(dataLabelsTable);
    }
    else dataLabelsTable = g_hash_table_new(g_str_hash, g_str_equal);

    usedRAMSpace = 0;
    signReg = UNKNOWN_SIGN;
}

void moveToNextTkn() {
    pToken = actToken;
    actToken = actToken->next;
}

void updateSign(int64_t x) {
    if (x > 0) signReg = POS;
    else if (x == 0) signReg = ZERO;
    else signReg = NEG;
}

void abortIfMemOverrun() {
    if (usedRAMSpace > MACHINE_MEMORY_SIZE_BYTES)
        throwError("Program used whole memory, not possible to allocate more", actToken->tkn.line);
}

void chopActNode() {
    removeNextNode(pToken);
    actToken = pToken->next;
}

void printHashNodeDataLabel(gpointer key, gpointer value, gpointer user_data) {
    // TODO: add arrays information
    printf("%s ::: %ld\n", (char*)key, *((MACHINE_BASIC_INT_TYPE*)value));
}

void printHashNodeCodeLabel(gpointer key, gpointer value, gpointer user_data) {
    printf("%s ::: %u\n", (char*)key, ((node_t*)value)->tkn.line );
}

uint32_t getCorrectLine(token_t x) {
    return x.line;
}
