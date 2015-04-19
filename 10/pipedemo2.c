#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define CHILD_MESS "I want a cookie\n"
#define PARENT_MESS "testing...\n"
#define oops(m,x) {perror(m); exit(x);}

int main(void) {
  int len, read_len, pipefd[2];
  char buf[BUFSIZ];

  if (pipe(pipefd) == -1)
    oops("cannot get a pipe", 1);

  switch(fork()) {
    case -1:
      oops("cannot fork", 2);
    case 0:
      len = strlen(CHILD_MESS);
      while (1) {
        if (write(pipefd[1], CHILD_MESS, len) != len)
          oops("cannot", 3);
        sleep(5);
      }
    default:
      len = strlen(PARENT_MESS);
      while (1) {
        if (write(pipefd[1], PARENT_MESS, len) != len)
          oops("write", 4);
        sleep(1);
        read_len = read(pipefd[0], buf, BUFSIZ); 
        if (read_len <= 0)
          break;
        write(1, buf, read_len);
      }
  }

  return EXIT_SUCCESS;
}
