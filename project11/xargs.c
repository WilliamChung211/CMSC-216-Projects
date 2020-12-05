#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "split.h"
#include <string.h>
#include "safe-fork.h"
#include <sys/wait.h>
#include <sysexits.h>

/*
 * Name: William Chung
 * Discusion Number:0101
 *
 * This file represent a program that represents a simulation
 * of the UNIX utility xargs. Xargs runs a program given arguments
 * in either standard mode (runs arguments in input) or line_at-a-time
 * mode (runs arguments for each line one at a time). It does this as
 * we have functions to represent this by making a parent and child 
 * and sending information through a pipe. In the functions, parents
 * send the child the arguments while the child runs them. The main 
 * functions decides what arguments are passed to these functions
 * and what function used based on the original arguments for xarsg.
 */

static void  standard_mode (char** argv, int arg_size, int argc);
static void exec_line(char** argv, int size, int argc, char* line);
static int line_mode(char** argv, int size, int argc);

/*Executes a target program once by taking the program arguments
already given and adding the new arguments from the input line.
It does this by using a pipe and forking. The parent thinks its
output pipe is the standard output, so it prints the number of
arguments and arguments by looking though each line. There the child
which thinksits input pipe is standard input, so it gets the arguments
will then call exec now that it has all the arguments. Exits with 
status 0 if it was successful in executing the program. Exits with
status 1 if it didn't*/ 
static void standard_mode (char** argv, int arg_size, int argc) {

  int pipefd [2];
  int pid;
  char** lines;
  char** line_words;
  int size=10;
  int i=0;
  int line_ind=0;
  char** args;
  int num_arg;
  int word_ind;
  char* line;
  int len;
  int child_stat;
  int num_lines=0;
  char *new_line_buff;

  /*Makes a pipe and forks the child*/
  pipe(pipefd);
  pid=safe_fork();
    
  if (pid  > 0) {

    
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
  
    lines=malloc(10 * sizeof(char*)); 

    if (lines == NULL) {
      free(argv);
      exit(1);
    }

    /*keeps going till eof happens*/
    while ( !feof(stdin) ) {

      /*Allocates more memory for the lines array if we
       hit the limit*/
      if (num_lines == size - 1) {
	size*=2;
	lines=realloc(lines, sizeof(char*) * size );

	if (lines == NULL) {
	  free(argv);
	  exit(1);
	}
	
      }

      /*Allocates memory for the line as the max of the character
	is assumed to be 1000 characters*/
      lines[num_lines]=malloc(1000 * sizeof(char));

      if (lines[num_lines] == NULL) {
	free(lines);
	free(argv);
	exit(1);
      }

      /*puts the line in the array. Reverses what is done if eof is hit.
	If not, the line counter is updated*/
      if (fgets(lines[num_lines], 1000, stdin) == NULL) {
	free(lines[num_lines]);
      }
      else{
	num_lines++;
      }
      
   }
  

    /*gives the number of lines through the pipe*/
   printf("%d\n", num_lines);

   /*Prints the original arguments passed into the function*/
   for (i=0;i < argc; i++) {
      printf(argv[i]);
      printf(" ");
   }


   /*Gets all the words from the lines and prints them out*/
   for (; line_ind<num_lines; line_ind++) {

     /*Splits up the words*/
     line_words=split(lines[line_ind]);
     free(lines[line_ind]);

     /*Prints all the arguments in the line*/
     for (word_ind=0; line_words[word_ind]!=NULL; word_ind++){
       printf(line_words[word_ind]);
       printf(" ");
       free(line_words[word_ind]);
       line_words[word_ind]=NULL;
     }

     free(line_words);

   }
          

   /*After printing out every argument from every line, prints
     everything out as one line*/
   free(lines);
   printf("\n");
   fflush(stdout);

   /*Waits till the child finishes executing */
   wait(&child_stat);
   
   free(argv);

   /*If child exits successful, exits with 0, if not
     exits with 1*/
   if (child_stat == 0)
     exit(0);
   else
     exit(1);

  }

  else if (pid == 0) {

    /*Gets input from the pipe*/
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    scanf("%d", &num_arg);

    /*Gets maximum size of what it has to get*/
    len=1001 * num_arg * sizeof(char) + arg_size + argc;
    line=malloc(len);
    new_line_buff=malloc(sizeof("\n"));
    if (line == NULL)
      exit(1);

    fgets(new_line_buff, sizeof("\n"), stdin);
    fgets(line, len, stdin);

    /*Gets the argument by splitting the entire line*/
    args=split(line);
    free(new_line_buff);
    free(line);

    /*Executes the program*/
    execvp(args[0], args);
    free(argv);
    free(args);
    exit(1);
   
  }

  /*Exits with 1 if a fork did not happen*/
  exit(1);

}


/* Given arguments, size of the arguments, the number of arguments,
and a line of the next arguments, executes a program with the
arguments. It does this by adding the initial arguments to a string,
and then adding all the arguments from the line parameter in the string,
and having the parent proccess send that to the child proccess. With
that info, the child proccess then reads that as an entire line, splits
it, and then runs exec. Exits with status 1 if program did not run
successfully.*/
static void exec_line(char** argv, int size, int argc, char* line) {

  int pipefd [2];
  int pid;
  char** args;
  char get_line [BUFSIZ];
  int mal_size;
  char *send;
  int i;
  int child_stat;

  /*Makes a pipe and forks a parent and child*/
  pipe(pipefd);
  pid=safe_fork();

  if(pid >0){

    close(pipefd[0]);

    /* gets size of the entire string it will send*/
    mal_size=sizeof(char) * ( size + strlen(" ") * argc +
			    strlen(line) + 1);

    send=malloc(mal_size);

    if (send == NULL) {
      free(line);
      free(argv);
      exit(1);
    }

    /*Intializes the variable*/
    strcpy(send,"") ;

    /*Puts all the initial arguments*/
    for (i=0;i < argc; i++) {
      strcat(send, argv[i]);
      strcat(send, " ");
    }
    
    /*Adds the entire line of arguments*/
    strcat(send, line);
 ;
    /*Writes the string of arguments to the pipe*/
    write(pipefd[1], send, mal_size);

    /*Waits till the execution of the program*/
    wait(&child_stat);
    free(send);

    /*If program did not run succesfully exits with status 1*/
    if(child_stat != 0)
      exit(1);
    
  }
  else if (pid == 0) {

    close(pipefd[1]);

    if(read(pipefd[0], get_line, BUFSIZ) == -1) {
      free(line);
      free(argv);
      exit(1);
    }
    else {

      /*The child splits the line it recieved to get the arguments
	and then runs the program */
      close(pipefd[0]);
      args=split(get_line);
      execvp(args[0], args);
      free(line);
      free(argv);
      free(args);
      exit(1);
    }
  
  }
  else 
    exit(1);
  
}

/*This function runs line-at-a-time mode which means that
it will execute each target program once for each standard input
line. It does this by calling the exec_line function which
first uses the already given arguments in the parameter of this
function (argv) and then adding the ones in the line and then
calling exec on it. It also gives the size of the initial arguments
and number of argumnets. It exits 0 if all programs were ran
succesfully and exits 1 if not.*/
static int line_mode(char** argv, int size, int argc) {

  char* line;
  
  line=malloc(1000 * sizeof(char));

  if (line == NULL)
    exit(1);

  /*Goes through every line till eof and executes a program
    given the initial arguments in addition with these line arguments*/
  while (fgets(line, 1000, stdin)) 
    exec_line(argv, size, argc, line);

  free(line);
  free(argv);
  exit(0);


}

/* Runs the xargs program given the inital arguments
   given in argc. If -i is given, it runs in line-at-a-time
mode which runs a program once fo reach line. If not given, it runs
standard mode which just runs once for all the arguments given. If more
arguments are given, it runs a program based off it. If not, it runs
"/bin/echo" by default." Exits 0 if runs program(s) successfully and
1 if not */
int main(int argc, char* argv[]) {

  int i;
  char** arg_par;
  int size;

  /*If there are no arguments, it runs echo on
    standard mode*/
  if (argc == 1) {
    arg_par=malloc(sizeof(char*));
    arg_par[0]="echo";
    size =4 * sizeof(char);
    standard_mode(arg_par, size, 1);
  }

  /*if an "-i" is included, it runs line-at-a-time mode*/
  else if (strcmp(argv[1], "-i") == 0) {

    /*if "-i" is the only argument, it runs echo*/
    if (argc <= 2) {
      arg_par=malloc(sizeof(char*));
      arg_par[0]="echo";
      size=sizeof(char) * 4;
      line_mode(arg_par, size, 1);
    }

    /*If not, it gets all the arguments and calls line_mode
      given those arguments*/
    else {     
      arg_par=malloc(sizeof(char*) * (argc - 2) );

      /*It also gets the size of the arguments*/
      for(i=2;i < argc;i++) {
	arg_par[i-2]=argv[i];
	size+=strlen(argv[i]);
      }

      size*=sizeof(char);
      line_mode(arg_par, size, argc - 2);
    }
    
  }

  /*Runs standard mode if there are arguments
    without "-i" as the 1st command line argument*/
  else{

    arg_par=malloc(sizeof(char*) * (argc - 1) );

    /*Gets all the arguments and also its sizes*/
    for(i=1;i < argc;i++) {
      arg_par[i-1]=argv[i];
      size+=strlen(argv[i]);
    }

    size*=sizeof(char);

    /*runs standard mode given those arguments*/
    standard_mode(arg_par, size, argc - 1);

  }
 
  return 0;

}
