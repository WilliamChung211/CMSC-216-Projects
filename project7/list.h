/*
 *Name: William Chung
 *ID: Wchung1
 *UID: 116609968
 *Discusion Number:0101
 *
 * This file is used to create the
 * node structure that is used for the 
 * list structure which is also created.
 * This specific list structure is implemented
 * a circular doubly linked list
 * using a dummy node and a size variable to
 * keep track of the number of elements. The
 * dummy node's previous is meant to be the
 * actual tail and next is meant to be the head.
 * The dummy node's data variable is never meant
 * to be accessed and is not part of the public
 * list to the client's knowledge.
 */

typedef struct node {
  int data;
  struct node *prev;
  struct node *next;
} Node;


typedef struct {
  int size;
  Node *dummy_node;
} List;
  
void init(List *const list);
int append(List *const list, int new_value);
int prepend(List *const list, int new_value);
int size(List *const list);
int find(List *const list, int value);
int delete(List *const list, unsigned int position);
void print(List *const list);
