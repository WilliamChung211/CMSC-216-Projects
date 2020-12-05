#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "student.h"

/*
 * Name: William Chung
 * Discusion Number:0101
 * This program and file has functions for
 * the student structure by using dynamic
 * memory allocation
 */


/*returns a new student using dynamic memory allocation*/
Student *new_student (const char name[], unsigned int id, float shoe_size) {
  Student *new_stud = malloc(sizeof(Student));

  if (new_stud != NULL) {
    if (name == NULL) {
      new_stud->name = malloc(sizeof(char));

      if (new_stud->name != NULL) 
	strcpy(new_stud->name, "");
      
   }
   else {
     new_stud->name = calloc((strlen(name) + 1), sizeof(char));

     if (new_stud->name != NULL)
       strcpy(new_stud->name, name);
     
   }
    
    new_stud->id = id;
    new_stud->shoe_size = shoe_size;
  }
  
  return new_stud;
}

unsigned int has_id (Student *const student, unsigned int id) {
  return student != NULL && (student->id) == id;
}

unsigned int has_name (Student *const student, const char name[]) {
  return student != NULL && name != NULL && strcmp(student->name, name) ==0;
}

unsigned int get_id (Student *const student) {
  return student == NULL ? 0 : student->id; 
}

float get_shoe_size (Student *const student) {
  return student == NULL ? 0 : student->shoe_size;
}

void change_shoe_size (Student *const student, float new_shoe_size) {
  if (student != NULL)
    student->shoe_size = new_shoe_size;
}


/*changes the name by freeing and allocating memory*/
void change_name (Student *const student, const char new_name[]) {

  if (student != NULL) {
    
    free(student->name);
    
    if (new_name == NULL) {
      
      student->name = malloc(sizeof(char));

      if (student->name != NULL) 
	strcpy(student->name, "");
     
    }
    else {
      
      student->name = calloc((strlen(new_name) + 1), sizeof(char));

      if (student->name != NULL)
	strcpy(student->name, new_name);
      
    }
    
  }
}

/*Makes student1 a copy of student2*/
void copy_student (Student *student1, Student *const student2) {

  if (student1 != NULL && student2 != NULL) {
    free(student1->name);
    free(student1);
    student1 = new_student ( student2->name, student2->id, student2 -> shoe_size);
  }
}
