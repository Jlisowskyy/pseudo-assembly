//
// Created by Jlisowskyy on 10/8/23.
//

#ifndef INTERPRETER_LIST_H
#define INTERPRETER_LIST_H

#include "token.h"

#define DATA_T token_t

typedef struct node{
    struct node* next;
    DATA_T tkn;
}node_t;

typedef struct list{
    node_t* head;
    node_t* tail;
}list_t;

list_t initList(DATA_T data);
list_t pushBack(list_t target, DATA_T data);
DATA_T getNextToken(node_t* extractionPoint);
void cleanList(list_t target);

#endif //INTERPRETER_LIST_H
