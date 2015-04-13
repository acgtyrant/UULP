#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAXARGS 20
#define ARGLEN  100

extern char *makestring(char argbuf[]);
extern void execute(char *arglist[]);

int main(void) {
  char *arglist[MAXARGS + 1];
  int numargs = 0;
  char argbuf[ARGLEN];
  while (numargs < MAXARGS) {
    printf("Arg[%d]?", numargs);
    if (fgets(argbuf, ARGLEN, stdin) == NULL)
      exit(EXIT_SUCCESS);
    else if (*argbuf != '\n')
      arglist[numargs] = makestring(argbuf);
    else {
      if (numargs > 0) {
        arglist[numargs] = NULL;
        execute(arglist);
        while (--numargs)
          free(arglist[numargs]);
        numargs = 0;
      }
    }
    numargs++;
  }
  return EXIT_SUCCESS;
}

extern char *makestring(char argbuf[]) {
  argbuf[strlen(argbuf) - 1] = '\0';
  char *cp = (char *)malloc(strlen(argbuf) + 1);
  if (cp == NULL) {
    fprintf(stderr, "no memory\n");
    exit(EXIT_FAILURE);
  }
  strcpy(cp, argbuf);
  return cp;
}

extern void execute(char *arglist[]) {
  if (strcmp(arglist[0], "exit") == 0 && arglist[1] == NULL)
    exit(EXIT_SUCCESS);

  int pid, exitstatus;

  pid = fork();
  switch(pid) {
    case -1:
      perror("fork failed");
      exit(EXIT_FAILURE);
    case 0:
      execvp(arglist[0], arglist);
      perror("execvp failed");
      exit(EXIT_FAILURE);
    default:
      while (wait(&exitstatus) != pid)
        ;
      printf("child exited with status %d, %d\n",
              exitstatus >> 8, exitstatus & 0377);
  }
}
