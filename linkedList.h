// Copyright haoyangy your_loginName
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * create a linkedNode in a linkedList to store alias
 */
typedef struct linkedNode {
    char* alias;
    char* command;
    struct linkedNode* next;
} node;

/**
 * create a LinkedList to link each linkedNode
 */
typedef struct linkedList {
    node* head;
} list;
void listInit(list*);
void listMatch(list*, char*);
void listPrintAll(list*);
void listInsert(list*, char*, char*);
char* listGet(list*, char*);
void freeList(list*);
