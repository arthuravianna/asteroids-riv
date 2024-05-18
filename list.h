#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef linked_list
#define linked_list

typedef struct linked_list_node {
    void *data;
    struct linked_list_node *next;
} Node;

typedef struct linked_list {
    Node *first;
    int len;
} List;


List* initList();
int push(List *l, void *data, size_t datasize);
int pop(List *l);
int removeNode(List *l, int pos);
void clear(List *l);
void printList(List *l, void (*fptr)(void *));

#endif