#include "stdio.h"
#include "string.h"
#define NAME_LEN 40

/*
 * Name: William Chung
 * Discussion Section Number: 0101
 * This program and file represents
 * a structure that represents a student
 * with a name, id, and shoe size.
 */
typedef struct{
  char name [NAME_LEN];
  unsigned int id;
  float shoe_size;
} Student;


/* creates a new student given parameters*/
Student new_student (const char name[], unsigned int id, float shoe_size) {
  Student new_stud;
  
  /*Either will be empty string or first 39 letters"*/
  if (name == NULL) {
    strcpy(new_stud.name, "");
  }
  else{
    strncpy(new_stud.name,name,39);
    new_stud.name[39] = '\0';
  }
 
  new_stud.id=id;
  new_stud.shoe_size=shoe_size;

  return new_stud;
}

/*Changes a student pointer given parameters*/
void init_student (Student *const student, const char name[], unsigned int id, float shoe_size) {

  if (student == NULL) 
    return;
    
   if (name == NULL) {
     strcpy(student->name,"");
   }
   else {
    strncpy(student->name, name,39);
    (student->name)[39] = '\0';
  }

   student->shoe_size = shoe_size;
   student->id=id;

}

/*checks if student has id*/
unsigned int has_id (Student student, unsigned int id) {
  return student.id == id;
}

/*checks if student has name*/
unsigned int has_name (Student student, const char name[]) {
  return name != NULL && strcmp(student.name, name) == 0;
}


unsigned int get_id (Student student) {
  return student.id;
}

float get_shoe_size (Student student) {
  return student.shoe_size;
}

Student change_shoe_size (Student student, float new_shoe_size) {
  student.shoe_size = new_shoe_size;
  return student;
}

void change_name (Student *const student, const char new_name[]) {

  if (student == NULL ) 
    return;

  /*Changes name to empty string or first 39 characters*/
  if (new_name == NULL) {
    strcpy(student->name, "");
  }
  else {
    strncpy(student->name, new_name, 39);
    (student->name)[39] = '\0';
  }
  
}
