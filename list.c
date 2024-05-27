#include "list.h"


List* initList() {
    List *l = (List*)malloc(sizeof(List));
    l->first = NULL;
    l->len = 0;

    return l;
}

// failure = 0, success = 1
int push(List *l, void *data, size_t datasize) {
    if (!l) return 0;
    
    Node *node = malloc(sizeof(Node));
    node->data = malloc(datasize);
    node->next = l->first;

    memcpy(node->data, data, datasize);
    
    l->first = node;
    l->len++;
    
    return 1;
}


// failure = 0, success = 1
int pop(List *l) {
    if (!l) return 0;
    if (l->len == 0) return 0;

    Node *aux = l->first;
    l->first = l->first->next;

    free(aux->data);
    free(aux);
    l->len--;
    
    return 1;
}

// failure = 0, success = 1
int removeNode(List *l, int pos) {
    if (!l) return 0;
    if (l->len == 0) return 0;
    if (pos >= l->len) return 0;

    Node *curr_node = l->first;
    if (pos == 0) {
        l->first = curr_node->next;
    } else {
		Node *prev_node;
		for(int i = 0; i < pos; i++){
			prev_node = curr_node;
			curr_node = curr_node->next;
		}

		prev_node->next = curr_node->next;
    }

    free(curr_node->data);
    free(curr_node);
    l->len--;

    return 1;
}

// frees the memory allocated for the list
void clear(List *l) {
    Node *aux = l->first;

    while (aux) {
        Node *next_node = aux->next;
        free(aux->data);
        free(aux);

        aux = next_node;
    }

    free(l);
}

// receives a function to print the content of the list
void printList(List *l, void (*fptr)(void *)) {
    Node *node = l->first;

    while (node != NULL) 
    { 
        (*fptr)(node->data); 
        node = node->next; 
    }
}
  
// // Function to print an integer 
// void printInt(void *n) { 
//    printf(" %d", *(int *)n); 
// } 
  
// // Function to print a float 
// void printFloat(void *f) { 
//    printf(" %f", *(float *)f); 
// } 

// int main() { 
//     List *l = initList(); 
  
//     // Create and print an int linked list 
//     unsigned int_size = sizeof(int);
//     int arr[] = {10, 20, 30, 40, 50}, i; 
//     for (i=4; i>=0; i--) 
//        push(l, &arr[i], int_size); 
//     printf("Created integer linked list is \n"); 
//     printList(l, printInt); 
//     printf("\n");

//     removeNode(l, 0);
//     removeNode(l, 50); // this should do nothing
//     removeNode(l, 2);
//     printList(l, printInt); 
//     printf("\n");

//     clear(l);
//     return 0; 
// } 