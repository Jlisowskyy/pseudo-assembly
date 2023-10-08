//
// Created by Jlisowskyy on 10/8/23.
//
#include <stdlib.h>
#include <stdio.h>

#include "../include/list.h"

list_t pushBack(list_t target, DATA_T data)
    // Assumes that target has at least one element and simply adds next node to the tail
{
    node_t* newNode = malloc(sizeof(node_t));
    newNode->tkn = data;
    newNode->next = NULL;

    target.tail->next = newNode;
    target.tail = newNode;
    return target;
}

DATA_T getNextToken(node_t *extractionPoint) {
    return extractionPoint->next->tkn;
}

void cleanList(list_t target) {
    node_t* actual = target.head;
    node_t* prev;

    while(actual){
        prev = actual;
        actual = actual->next;
        free(prev);
    }
}

list_t initList(token_t data) {
    list_t result;
    node_t* newNode = malloc(sizeof(node_t));

    newNode->next = NULL;
    newNode->tkn = data;
    result.tail = result.head = newNode;
    return result;
}

void printTknList(list_t target) {
    size_t tokenCount = 0;
    node_t* p = target.head;

    while(p){
        PRINT_FUNC(p->tkn);

        if (p->next) printf(" --> ");
        p = p->next;

        if (tokenCount++ % 5 == 0) printf("\n");
    }
}

void printToken(token_t x) {
    static const char* representationTable[] = {
            "UNKNOWN",
            "IDENTIFIER",
            "REGISTER",
            "LABEL",
            "CONSTANT",
            "LINE_SEP"
    };

    printf("{ token type: %s ", representationTable[x.type]);

    switch (x.type) {
        case IDENTIFIER:
            printf(",%s ", x.strVal);
            break;
        case REGISTER:
            printf(",%ld ", x.numVal);
            break;
        case LABEL:
            printf(",%s: ", x.strVal);
            break;
        case CONSTANT:
            printf(",%ld ", x.numVal);
            break;
        default:
            break;
    }

    printf("}");
}
