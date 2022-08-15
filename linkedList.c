// Copyright haoyangy your_loginName
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linkedList.h"

void listInit(list* myList) {
    myList->head = NULL;
}
void listMatch(list* myList, char* alias) {
    node *tempNode = myList->head;
    while (tempNode != NULL) {
        //print out match alias
        if (strcmp(tempNode->alias, alias) == 0) {
            write(1, alias, strlen(alias));
            write(1, " ", strlen(" "));
            write(1, tempNode->command, strlen(tempNode->command));
            write(1, "\n", strlen("\n"));
            return;
        }
        tempNode = tempNode->next;
    }
}
/**
 * remove a Node which match the target alias
 * @param myList: the linkedList to do some work
 * @param alias: the alias to remove
 */
void listRemove(list* myList, char* alias) {
    node* tempNode = myList->head;
    //check head match
    if (strcmp(tempNode->alias, alias) == 0) {
        myList->head = tempNode->next;
        free(tempNode->alias);
        free(tempNode->command);
        free(tempNode);
    } else {
        //if not match head
        //current-iterateNode, tempNode-to store temp node from current
        node* currentNode = myList->head;
        while (currentNode->next != NULL) {
            if (strcmp(currentNode->next->alias, alias) == 0) {
                tempNode = currentNode->next;
                currentNode->next = currentNode->next->next;
                free(tempNode->alias);
                free(tempNode->command);
                free(tempNode);
                break;
            } else {
                currentNode = currentNode->next;
            }
        }
    }
}

void listPrintAll(list *myList) {
    node *tempNode = myList->head;
    while (tempNode != NULL) {
        write(1, tempNode->alias, strlen(tempNode->alias));
        write(1, " ", strlen(" "));
        write(1, tempNode->command, strlen(tempNode->command));
        write(1, "\n", strlen("\n"));
        tempNode = tempNode->next;
    }
}

void listInsert(list *L, char *alias, char *command) {
    node *new = malloc(sizeof(node));
    assert(new);
    new->alias = malloc(strlen(alias) + 1);
    strncpy(new->alias, alias, strlen(alias)+1);
    new->command = malloc(strlen(command) + 1);
    strncpy(new->command, command, strlen(command)+1);
    new->next = L->head;
    L->head = new;
}
char* listGet(list *myList, char *alias) {
    alias = strtok(alias, "\n");
    node *temp = myList->head;
    while (temp) {
        if ((strcmp(temp->alias, alias)) == 0) {
            return temp->command;
        }
        temp = temp->next;
    }
    return NULL;
}

void freeList(list *L) {
    node *ptr = L->head;
    node *temp;
    // int commandLen = 0;
    while (ptr) {
        temp = ptr;
        ptr = ptr->next;
        free(temp->alias);
        free(temp->command);
        free(temp);
    }
    free(L);
}
