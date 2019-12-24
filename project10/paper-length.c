#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "safe-fork.h"

/*
 * Name: William Chung
 * ID: Wchung1
 * UID: 116609968
 * Discusion Number:0101
 *
 * This file forks a child to read a paer, 
 * input its word count to the parent, and hve
 * the parent write a response based on it
 */


int main(void) {

  int wc;
  int pid;
  int fd[2];
  pipe(fd);
  pid = safe_fork();

  if (pid > 0) {
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    close(fd[1]);
    scanf("%d", &wc);

    if (wc >= 200) {
      printf("Long enough!\n");
      exit(0);
    }
    else {
      printf("Too short!\n");
      exit(1);
    }
    
  }
  else if ( pid == 0){
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    execlp("wc", "wc", "-w", NULL);
    printf("Couldn't execute.\n");
    exit(-1);
  }
  else{
    printf("Couldn't fork\n");
    exit(-1);
  }

  return 0;
  
}
