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

DATA_T getNextToken(node_t *extractionPoint)
    // Does not check existence of next node, expected usage: checking two nods synchronously
{
    return extractionPoint->next->tkn;
}

void cleanList(list_t target)
    // de-allocation of list
{
    node_t* actual = target.head;
    node_t* prev;

    while(actual){
        prev = actual;
        actual = actual->next;
        free(prev);
    }
}

list_t initList(token_t data)
    // simple list-factory
{
    list_t result;
    node_t* newNode = malloc(sizeof(node_t));

    newNode->next = NULL;
    newNode->tkn = data;
    result.tail = result.head = newNode;
    return result;
}

void removeNextNode(node_t *ptr)
// assumes everything is working correctly, which means, next and next next pointer existence is guaranteed
// due to existence of sentinel. Used to remove labels from token stream
// in human intelligence we trust
{
    node_t* temp = ptr->next->next;
    free(ptr->next);
    ptr->next = temp;
}

#ifdef DEBUG_

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
        case LINE_SEP:
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

#endif // DEBUG_
