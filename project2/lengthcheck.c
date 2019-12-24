#include <stdio.h>

/*
 *Name: William Chung
 *
 *This program checks for lines that are more than 80 characters.
 *This file helps with that by using character arrays to store
 *the characters to help notify if there is more than 80 
 *characters and prints it out.
 */

int main(void){

  char char_arr [999] = {0};
  int char_num =0;
  char character = ' ';
  int ind =0;
  
  /*Reads until end of input is seen*/
  while (scanf("%c", &character) != EOF) {

    /*Reads one line at a time*/
    while (character != '\n') {

      /*stores character in the line in an array to been printed later*/   
      char_arr[char_num] = character;
      char_num++;
      scanf("%c", &character);
   
    }

   /*Prints if the line is 80 characters or less and its size*/ 
   if (char_num <= 80) {
     printf(" ");
   }
   else {
     printf("X");    
   }
    
   printf(" %3d: ", char_num);

   /*prints all the characters*/
   while (ind < char_num) {
     printf("%c", char_arr[ind]);
     ind++;
   }

   printf("\n");
   char_num=0;
   ind=0;
    
 }

  return 0;
  
}
     
   
  
 
  
