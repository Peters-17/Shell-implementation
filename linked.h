//Copyright haoyangy __your_loginName
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct node {
    char *alias;
    char *command;
    struct node* next;
} node;

typedef struct linked {
    sttuct node* head;
} linked;

void linkedInit(linked*);
void linkedWrite(linked*, char*);
void linkedInsert(linked*, char*, char*);
char* linkedGet(linked*, char*);
void linkedSearch(linked*, char*);
void linkedRemove(linked*, char*);
void linkedPrint(linked*);
void linkedFree(linked*);

