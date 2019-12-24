/*
Name: William Chung
Login: Wchung1
ID Number: 116609968
Section Number: 0101

The program prints number of lines,  words,
and characters in total in all files by
concurrently finding the number of lines, words,
and characters in each file. 
*/

#include <stdio.h>
#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>


void * count_thread (void * args) {

  char ch, next_ch, *filename;
  int lines, words, chars;
  int* ret_info = malloc(3 * sizeof(int));
  FILE *fp;
  filename= args;
  
  fp= fopen(filename, "r");  /* open that file */

  /* silently ignore any problems trying to open files */
  if (fp != NULL) {
    lines= words= chars= 0;

    /* read each file one character at a time, until EOF */
    ch= fgetc(fp);

    while (!feof(fp)) {

      next_ch= fgetc(fp);  /* look ahead and get the next character */
      ungetc(next_ch, fp);  /* unread the next character (see Chapter 15) */

      /* update the counts as needed every time a character is read */

      /* a newline means the line count increases */
      if (ch == '\n')
          lines++;
      /* if the current character is not whitespace but the next character
	 is, or if the current character is not whitespace and it is the
	 last character in the input, the word count increases */
      if (!isspace(ch) && (isspace(next_ch) || feof(fp)))
	words++;
      
      /* increasing the character count is a no-brainer */
      chars++;
      
      ch= fgetc(fp);
    }
    
    /* add the totals for the current file into the accumulating totals
       for all files */
      ret_info[0]= lines;
      ret_info[1]= words;
      ret_info[2]= chars;
      
      fclose(fp);
  }

  return ret_info;
}

int main(int argc, char *argv[]) {

  int total_lines= 0;
  int total_words= 0;
  int total_chars= 0;
  int arg_num= 1; 
  int thread_num;
  pthread_t * threads= malloc((argc-1) * sizeof (pthread_t));
  int *ret_arr;
  void *ret;
  
  /* note that we skip argv[0], the name of the program being run */
  while (arg_num < argc) {  /* for each command-line argument */
    pthread_create(&threads[arg_num-1], NULL, count_thread, argv[arg_num]) ;
    arg_num++;
  }
 
  for (thread_num= 0; thread_num < argc - 1; thread_num++) {
    pthread_join(threads[thread_num], &ret);
    ret_arr=ret;
    total_lines+=ret_arr[0];
    total_words+=ret_arr[1];
    total_chars+=ret_arr[2];
    free(ret_arr);
  }

  free(threads);
  
  printf("%4d %4d %4d\n", total_lines, total_words, total_chars);
  
  return 0;
}


