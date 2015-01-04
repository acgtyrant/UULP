#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

int main(int argc, char *argv[]) {
  struct termios info;

  if (argc != 2)
    exit(EXIT_FAILURE);

  if (tcgetattr(0, &info) == -1) {
    perror("tcgetattr");
    exit(EXIT_FAILURE);
  }

  if (argv[1][0] == 'y') {
    info.c_lflag |= ECHO;
  }
  else {
    info.c_lflag &= ~ECHO;
  }

  if (tcsetattr(0, TCSANOW, &info) == -1) {
    perror("tcsetattr");
    exit(EXIT_FAILURE);
  }
  
  return EXIT_SUCCESS;
}
