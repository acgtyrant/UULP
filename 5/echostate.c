// echostate.c
// reports current state of echo bit in tty driver for fd 0 shows how to read
// attributes from driver and test a bit

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

int main(int argc, char *argv[]) {
  struct termios info;

  if (tcgetattr(0, &info) == -1) {
    perror("tcgetattr");
    exit(EXIT_FAILURE);
  }

  if (info.c_lflag & ECHO) {
    printf("echo is on, since its bit is 1\n");
  }
  else {
    printf("echo is off, since its bit is 0\n");
  }
  return EXIT_SUCCESS;
}
