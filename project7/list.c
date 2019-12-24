#include "list.h"
#include <stdlib.h>
#include <stdio.h>

/*
 *Name: William Chung
 *ID: Wchung1
 *UID: 116609968
 *Discusion Number:0101
 *
 *This project makes a linked list
 *structure by using dynamic memory
 *to allocate its nodes. The file represents
 *a doubly circular linked list with a dummy
 * node and has functions associated with it.
 * The dummy node's next points to the actual
 * head. The dummy node's previous points to
 *the actual tail of the list and does not need to
 * stores an actual value.  The list also
 *has a size variable to make other functions
 *run faster.
 */

/*Initialzizes the list by allocating memory for  the dummy node*/
void init (List *const list) {

  if (list != NULL) {

    /*an empty list has a size of 0 and the dummy points to itself*/
    list->size=0;
    list->dummy_node=malloc(sizeof(Node));

    if (list->dummy_node != NULL) {
      list->dummy_node->prev=list->dummy_node;
      list->dummy_node->next=list->dummy_node;
    }
    
  }
  
}

/*adds a new integer to the end of the list*/
int append(List *const list, int new_value){

  Node* to_add;

  /*if errors could happen returns 0*/
  if (list == NULL || ( to_add=malloc(sizeof(Node)) ) == NULL ) 
    return 0;

  /*makes toAdd the new tail and connects what is needed*/
  to_add->data = new_value;
  to_add->prev= list->dummy_node->prev;
  to_add->next=list->dummy_node;
  to_add->prev->next=to_add;
  list->dummy_node->prev =to_add;

  /*increments size and returns 1 for successful adding*/
  (list->size)++;
  return 1;

}

/*Adds a new first element to the beggining of the lniked list*/
 int prepend (List *const list, int new_value) {

  Node* to_add;
  if (list == NULL || ( to_add=malloc(sizeof(Node)) ) == NULL )
     return 0;

  /*makes the node the new actual head and makes connections needed*/
  to_add->data = new_value;
  to_add->prev=list->dummy_node;
  to_add->next=list->dummy_node->next;
  list->dummy_node->next=to_add;
  to_add->next->prev =to_add;

  (list->size)++;
  return 1;

}

/*Returns the number of elements if the list is a valid list*/
 int size (List *const list){
   return list==NULL ? 0 : list->size;
 }

/*Prints all elements of list on the same output line*/
void print (List *const list) {

  Node *current;
  
  if (list != NULL) {

    /*ignores dummy node since it's not part of the public list*/
    current = list->dummy_node->next;

    /*if the list is empty prints nothing*/
    if (current == list->dummy_node) 
      return;

    /*keeps printing till we reach the last element*/
    while (current->next != list->dummy_node) {
      printf("%d ", current->data);
      current = current->next;
    }

     /*we then print the last element with a new line instead of a space*/
    printf("%d\n", current->data);
  
  }

 

}

/*Finds the index of the first occurence of value*/
int find (List *const list, int value) {

  int index = 0;  
  Node *current;

  if (list == NULL)
    return 0;

  /*we start at the head*/
  current=list->dummy_node->next;

  /*we keep going till we traversed all elements*/
  while (index < list->size) {

    /*returns index if found*/
    if (current->data == value) {
      return index;
    }
    
    index++;
    current=current->next;
    
  }

  /*if we didn't find value, returns -1*/
  return -1;
}

/*removes a node the at position*/
int delete (List *const list, unsigned int position) {

  Node* current;
  int index = 0;

  /*returns 0 if list is null or position is out of bounds*/
  if (list == NULL || position < 0 || position >= list->size)
    return 0;

  /*stars at the first element of list*/
  current = list->dummy_node->next;
    
    while (current != list->dummy_node) {

      if (index == position) {

	/*makes connections to get rid of any trace of the deleted node*/
	current->next->prev=current->prev;
	current->prev->next=current->next;
	current->next = NULL;
	current->prev = NULL;

	/*frees the memory allocated for that deleted node*/
	free(current);
	(list->size)--;
	return 1;
      }
    
      index++;
      current=current->next;
    
  }

  return 0;
}
