#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  char *arglist[3];
  arglist[0] = "ls";
  arglist[1] = "-l";
  arglist[2] = NULL;
  printf("*** About to exec ls -l\n");
  execvp("ls", arglist);
  printf("*** ls is done. bye\n");
  return EXIT_SUCCESS;
}
