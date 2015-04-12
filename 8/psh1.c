// 教材版 psh1.c 有内存泄漏……

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
    if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n') {
      arglist[numargs] = makestring(argbuf);
    }
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
  execvp(arglist[0], arglist);
  perror("execvp failed");
  exit(EXIT_FAILURE);
}
