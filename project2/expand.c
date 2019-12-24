#include <stdio.h>

/*
 *Name: William Chung
 *
 *The project is used to check for lines more than 80 characters.
 *This file helps with it because replaces each tab with a number
 * of spaces to represent it so that the other file can calculate
 *printed length
 */


int main(void) {
  
  int char_ind=0;
  int blank_space =1;
  int ind=0;
  char character = ' ';

  /*Reads till end of its input is seen*/
  while (scanf("%c", &character) != EOF) {

    /*Reads one line at a time*/
    while (character != '\n') {

      if (character == '\t') {

	/*the tab occupies up to the next factor of 8  position*/
	blank_space+= ( 8 - (char_ind + 1) % 8) % 8;

	/*if it is a tab, it prints out a number of blank spaces instead*/
	while (ind < blank_space) {
	  
	  printf(" ");
	  ind++;
	  char_ind++;
	  
	}
	
	blank_space=1;
	ind=0;
	
      }

      /*prints normally if not a tab*/
      else {
	char_ind++;
	printf("%c", character);
      }
      
      scanf("%c", &character);
      
    }
    
    printf("\n");
    char_ind=0;
    
  }

  return 0;
  
}
