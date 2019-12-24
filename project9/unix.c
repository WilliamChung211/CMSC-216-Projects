
#include "unix.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int make_new_node(Unix *filesystem, const char arg[],
			 int is_dir, int not_err);
static void free_node (Node * head);
static void print_path (Node *node);
/*
 * Name: William Chung
 * ID: Wchung1
 * UID: 116609968
 * Discusion Number:0101
 *
 * This program represents a filesystem.
 * The file includes functions that use the
 * data structure that repesents the system
 * to simulate its commands of making components
 * and printing information. Now, it has new functions
 * so that you can print a path or remove/deallocate nodes
 * or the entire filesystem. 
 */

/*Initializes the filesystem
  by creating the root directory in it
  by making a new node and setting it as
  the variable.*/
void mkfs(Unix *filesystem) {

  Node *new_node;

  if (filesystem != NULL) {

    new_node=malloc(sizeof(Node));

    if (new_node == NULL)
      printf("Not enough memory");
    else {

      /*a root will print "/" when pwd is called on it*/
      new_node->name=malloc(sizeof(char) * 2);
      strcpy(new_node->name, "/");

      if (new_node->name == NULL) {
	printf("Not enough memory");
	return;
      }

      /*a root's parent is itself*/
      new_node->parent=new_node;
      new_node->child=NULL;
      new_node->next=NULL;
      new_node->is_dir=1;
      filesystem->current_dir=new_node;
    }     
  }

}

/*Makes a new node in the directory. Returns 1 if it did or not_error
is true. Returns 0 if not.*/
static int make_new_node(Unix *filesystem, const char arg[], int is_dir,
                         int not_err) {

  Node* current;
  Node* new_node;

  current=filesystem->current_dir->child;

  /*if nothing's in the directory, makes the first child*/
  if (current == NULL) {
    filesystem->current_dir->child=malloc(sizeof(Node));
    new_node = filesystem->current_dir->child;
  }
  else {

    /*returns a value if it's already in the directory*/
    if (strcmp (current->name, arg) == 0)
      return not_err;

    /*inserts first if it's first alphabetically*/
    if (strcmp (current->name, arg) > 0) {
      new_node=malloc(sizeof(Node));

      if (new_node == NULL) {
        printf("Not enough memory");
        return 0;
      }


      new_node->name=calloc(strlen(arg) + 1, sizeof(char));

      if (new_node->name == NULL) {
        printf ("Not enoguh memory");
        return 0;
      }

      strcpy(new_node->name, arg) ;
      new_node->next=filesystem->current_dir->child;
      new_node->child=NULL;
      new_node->is_dir=is_dir;
      new_node->parent=filesystem->current_dir;
      filesystem->current_dir->child=new_node;
      return 1;
    }

    /*goes through each component till the last*/
    while (current->next != NULL) {

      if (strcmp(current->next->name, arg) == 0) {
        return not_err;
      }

      /*inserts in the middle if it needs to be inserted*/
      if (strcmp(current->next->name, arg) > 0) {
        new_node=malloc(sizeof(Node));

        if (new_node == NULL) {
          printf("Not enough memory");
          return 0;
        }

        new_node->name=calloc(strlen(arg) + 1, sizeof(char));
        if (new_node->name == NULL) {
          printf ("Not enough memory");
          return 0;
        }

        strcpy(new_node->name, arg) ;
        new_node->next=current->next;
        new_node->child=NULL;
        current->next=new_node;
        new_node->is_dir=is_dir;
        new_node->parent=filesystem->current_dir;
        return 1;
      }

      current = current->next;
    }

    /*make a new last node in the directory*/
    current->next=malloc(sizeof(Node));
    new_node=current->next;
  }

  /*checks if there is enough memory*/
  if (new_node == NULL) {
    printf("Not enough memory");
    return 0;
  }

  /*makes the link to make this node the last node in the directory*/
  new_node->name=calloc(strlen(arg) + 1, sizeof(char));

  if (new_node->name == NULL) {
    printf("Not enough memory");
    return 0;
  }
  strcpy(new_node->name, arg) ;
  new_node->next=NULL;
  new_node->child=NULL;
  new_node->is_dir=is_dir;
  new_node->parent=filesystem->current_dir;
  return 1;

}

/*makes a new file in the directory by maknig a new
non-directory node.  Returns 0 if it is a
specific error case or invalid parameter. 
Returns 1 if it doesn't.*/
int touch(Unix *filesystem, const char arg[]) {
  if (filesystem == NULL || arg == NULL)
    return 0;

  if (strcmp(arg,".") == 0 || strcmp (arg, "..") == 0
      || strcmp(arg, "/") ==0)
    return 1;

  /* these are the only specific error cases*/
  if ( (strchr(arg, '/') != NULL) || strcmp(arg, "") == 0 )
    return 0;

  /*makes a new file*/
  return make_new_node (filesystem, arg, 0, 1);
}

/*makes a new subdirectory in the current directory by
making a new subdirectory node. Returns 1 if it did. Returns
0 if it failed.*/
int mkdir(Unix *filesystem, const char arg[]) {

  if (filesystem == NULL || arg == NULL)
    return 0;

  /*these are specific error cases*/
  if (strcmp(arg,".") == 0 || strcmp (arg, "..") == 0
      ||  strcmp(arg, "") == 0 || strchr(arg, '/') != NULL)
    return 0;

  /*Makes a new directory node*/
  return make_new_node (filesystem, arg, 1, 0);

}

/* Changes the current directory in the unix parameter
by finding the correct directory inside the current directory
and updating the filesystem's instance variable. Returns 1
if it could find a directory in the filesystem. 0 if not*/
int cd(Unix *filesystem, const char arg[]) {

  Node *current;

  if (filesystem == NULL || arg == NULL)
    return 0;
  /* It does nothing if arg is "."*/
  if (strcmp(arg,".") != 0){ 

    /* Changes the current directory's parent*/
    if (strcmp(arg,"..") == 0) {
      filesystem->current_dir = (filesystem->current_dir->parent);
    }

    /* the arg "/" changes to the root directory */
    else if (strcmp(arg, "/") == 0) {

      /*The root is the  only node  where its "parent" is itself*/
      while (filesystem->current_dir != filesystem->current_dir->parent) 
	filesystem->current_dir=filesystem->current_dir->parent;
    }

    else{

      /*switches to subdirectory in this directory*/
      current=filesystem->current_dir->child;

      /* can't switch if the directory has nothing*/
      if (current == NULL) {
	return 0;
      }
      
      else {

	/* keeps looking in the sorted nodes till it shouldn't*/
	while ( ( current != NULL) && strcmp(current->name, arg) <= 0 ) {

	  /*only changes if it found the name and it is a directory*/
	  if (strcmp(current->name, arg) == 0) {
	    
	    if (current->is_dir) {
	      filesystem->current_dir=current;
	      return 1;
	    }
	    else {
	      return 0;
	    }
	
	  }
	  
	  current = current->next;
	}

	/*if it can't find the directory, it can't change*/
	return 0;
      }

    }

   
  }

  /*changed directory for the special arguments is true*/
  return 1;
}

/*prints content of the directory each seperated by
a new line and also a foward slash after the name if
it is a directory. Won't print anything if there is nothing
Returns 1 if it found the argument */
int ls(Unix *filesystem, const char arg[]) {

  Node* directory;
  Node* current;

  if (filesystem == NULL || arg == NULL)
    return 0;

  /*prints all contents of current directory*/
  if (strcmp(arg, "") == 0 || strcmp(arg, ".") == 0)
    directory = filesystem->current_dir;
  /*prints all contents of current directory's parents*/
  else if (strcmp(arg, "..") == 0) {
    directory=filesystem->current_dir->parent;
  }

  /*prints all contents of the root directory*/
  else if (strcmp(arg, "/") == 0) {

    directory = filesystem->current_dir;

    /*only the root directory has itself as its parent*/
     while (directory != directory->parent)
        directory=directory->parent;

  }
  else {

    directory = filesystem->current_dir->child;

    /*goes through all nodes till it founds it or it does not need to*/
    while ((directory != NULL) && strcmp(directory->name, arg) < 0) {
      directory=directory->next;
    }

    /*if it could not find it, it can't list anything*/
    if (directory == NULL || strcmp(directory->name, arg) >  0)
      return 0;

    /*if the thing it found was a fie, it prints out the file only.*/
    if(!directory->is_dir) {
      printf("%s\n", directory->name);
      return 1;
    }

  }

  current = directory->child;

  /*prints everything in the directory alphabetically*/
  while (current != NULL) {
    printf("%s", current->name);

    if (current->is_dir) {
      printf("/");
    }

    printf("\n");
    current = current->next;
  }

  return 1;
}


/* This function prints the node's entire path
by going up each parent of the node till it gets
to the root directory and then after, printing
all the names (seperated by foward slash). Assumes
the node paramter is valid.
 */
static void print_path (Node *node){

  /*if it's not the root keeps going up*/
  if (node->parent != node) {
    print_path(node->parent);

    /*Eventually prints the name*/
    printf(node->name);
  }

  /*Seperated by foward slash*/
  printf("/");
  
}

/*Prints the full path from the root to the current
directory. It uses foward slashes to seperate names of
the path except on the last name. The a new line is
printed.*/
void pwd(Unix *filesystem) {

  /*Returns and does nothing if invalid parameter  is given*/
  if (filesystem != NULL) {

    /*Prints a foward slash and new line only if it's the root dir*/
    if (strcmp(filesystem->current_dir->name, "/") == 0) {
      printf("/");
    }

    /*Prints the path sperated by foward slashes if not
     by calling this recursive function to print all its
     parents and then prints itself and then the newline*/
    else {
      print_path(filesystem->current_dir->parent);
      printf(filesystem->current_dir->name);
    }
    
    printf("\n");
  }
  
}

/*Deallocated all dynamically-allocated memory used 
  by this parameter by freeing every file/directory
  that the root directory contains and then the root
  directory itself*/
void rmfs (Unix *filesystem) {

  /*does nothing if paramter is invalid*/
  if (filesystem != NULL) {

    /*Starts for the root directory*/
    cd(filesystem, "/");

    /*By calling this function it removes everything
      in the root directory*/
    free_node (filesystem->current_dir->child);

    /*It then frees the root directory itself and makes 
      it so the there is no dangling pointer by setting
      file system's node till null*/
    free(filesystem->current_dir->name);
    free(filesystem->current_dir);
    filesystem->current_dir=NULL;
  }
  
}

/* Frees all the nodes neighbors and its children 
   and its associated names till everything is free*/
static void free_node (Node * head) {

  Node *current = head;
  Node *next;

  /*Bae case is if there is no node*/
  while (current != NULL) {

    /*It first much free all it's children*/
    free_node(current->child);

    /*It then frees the name, itself, and goes to its neighbor*/
    free(current->name);
    next=current->next;
    free(current);
    current=next;
  }
  
}

/*Returns 0 if error condition like the  name is not located
in the directory. Return 1 if successful in removing the file
or directory. It does this by searching the current directory
for the argument, then freeing  all its files/subdirectories 
if it was a directory, and then freeing and unlinking itself.*/ 
int rm (Unix *filesystem, const char arg[]) {

  Node *current;
  Node *previous;
  Node *new_next;

  /*Returns 0 if it i an error case*/
  if (filesystem == NULL  || arg == NULL) 
    return 0;

  if (strcmp(arg,".") == 0 || strcmp (arg, "..") == 0
      ||  strcmp(arg, "") == 0 || strchr(arg, '/') != NULL)
    return 0;

  
  /*Stars with the first thing in this directory*/
  current=filesystem->current_dir->child;

  /* can't remove anything  if the directory has nothing*/
  if (current == NULL) {
     return 0;
  }

  else {

    /*The case if the first file/directory is the argument*/
    if (strcmp(current->name, arg) == 0) {

      new_next=current->next;

      /*Deallocates all children and itself*/
      free_node(current->child);
      free(current->name);
      free(current);

      /*Updates the filesystem's next first child*/
      filesystem->current_dir->child=new_next;
      return 1;
    }

    previous=current;
    current=current->next;
    
    /* keeps looking in the sorted nodes till it shouldn't*/
    while ( ( current != NULL) && strcmp(current->name, arg) <= 0 ) {
      
      /*only deallocates if it found the name*/
      if (strcmp(current->name, arg) == 0) {
	new_next=current->next;

	/*Deallocates the node's children (or lack of)*/
	free_node(current->child);

	/*Deallocates itself*/
	free(current->name);
	free(current);

	/*Links the deleted node's neighbors together*/
        previous->next=new_next;

	/*Returns a successful removing*/
	return 1;
      }

      previous = current;
      current = current->next;
    }

    /*if it can't find the argument, it can't remove anything
     so it failed*/
    return 0;
  }  
  
}
