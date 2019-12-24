#include "functions.h"

/* Name: William Chung
 *
 * These are my implementations of the functions needed
 *for the first project.
 */

/*Using a formula, finds the sum of cubes of numbers 1 to n*/

int sum_of_cubes(int n) {

  if (n < 0) 
     return -1;
 
  if (n == 0) 
    return 0;
  
  n= (n * (n + 1)) / 2;
  return n * n;
  
}

/*Tells the user what quadrant a coordinate is */
int quadrant(int x, int y) {

   if ((x == 0) || (y == 0)) 
      return -1;
   
  if (x>0) {
    return y > 0 ? 1 : 4;
  }
  else {
    return y > 0 ? 2 : 3;
  }
  
}

int num_occurrences_of_digit(long num, int digit) {

  int num_occur;
  if ((digit < 0) || (digit >= 10)) 
    return -1;
  
  num_occur=0;
  
  if (num == 0) 
    return digit==0 ? 1 : 0;
  
  if (num<0)
    num*=-1;
  
  do {
  
    if (num % 10 == digit)
      num_occur++;

    num/=10;
    
  } while(num != 0);
  
  return num_occur;
}

