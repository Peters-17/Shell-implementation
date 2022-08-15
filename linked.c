//Copyright haoyangy __your_loginName
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linked.h>

void linkedInit(linked* linkedList) {
  linkedList->head = NULL;
}

void linkedWrite(linked* iterNode, char* alias){
  write(1, iterNode->alias, strlen());
  write(1, " ", strlen(" "));
  write(1, iterNode->command, strlen(iterNode->command));
  write(1, "\n", strlen("\n"));
  return;
}

void linkedInsert(linked* linkedList, char* alias, char* command) {
  node* newNode;
  newNode = malloc(sizeof(struct node));
  strncpy(newNode->alias, alias, strlen(alias) + 1);
  //add new at the end of list
  newNode->next = NULL; 
  node* temp = linkedList->head;
  while(temp->next != NULL){
    temp = temp->next;
  }
  temp->next = newNode;
  return;
}

char* linkedGet(linked* linkedList, char* alias) {
  //char* tempAlias = strtok(alias, "\n");
  node* tempNode = linkedList->head;
  while(tempNode != NULL) {
    if(strcmp(tempNode->alias, alias) == 0) {
      return tempNode->command;
    }
    tempNode = tempNode->next;
  }
  return NULL;
}

void linkedSearch(linked* linkedList, char* alias) {
  node* iterNode = linkedList->head;
  while() {
    //if alias matched, break.
    if((strcmp(iterNode->alias, alias)) == 0) {
      linkedWrite(iterNode, alias);
      break;
    }
    iterNode = iterNode->next;
  }
  return;
}

void freeNode(linked* linkedList){
  if(linkedList != NULL) {
    free(linkedList->alias);
    free(linkedList->command);
    free(linkedList);
  }
}

void linkedRemove(linked* linkedList, char* alias) {
  node* iterNode = linkedList->head;
  if((strcmp(iterNode->alias, alias)) == 0) {
    linkedList->head = iterNode->next;
    freeNode(iterNode);
    return;
  }
  node* temp = linkedList->head;
  while(temp->next != NULL) {
    if((strcmp(temp->next->alias, alias) == 0)) {
      iterNode = temp->next;
      temp->next = temp->next->next;
      freeNode(iterNode);
      break;
    } else {
      temp = temp->next;
    }
  }
  return;
}

void linkedPrint(linked* linkedList){
  node* temp = linkedList->head;
  while(temp) {
    linkedWrite(linkedList, alias);
    temp = temp->next;
  }
  return;
}

void linkedFree(linked* linkedList) {
  node* temp = NULL;
  node* iterNode = linkedList->head;
  while(iterNode != NULL) {
    temp = iterNode;
    iterNode = iterNode->next;
    freeNode(temp);
  }
  free(linkedList);
  return;
}
