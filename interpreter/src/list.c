//
// Created by Jlisowskyy on 10/8/23.
//
#include <stdlib.h>

#include "../include/list.h"

list_t pushBack(list_t target, token_t data)
    // Assumes that target has at least one element and simply adds next node to the tail
{
    node_t* newNode = malloc(sizeof(node_t));
    newNode->tkn = data;

    target.tail->next = newNode;
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
