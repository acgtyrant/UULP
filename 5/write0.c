// write0.c
//
// purpose: send messages to another terminal
//  method: open the other terminal for output then copy from stdin to that
//          terminal
//    show: a terminal is just a file supporting regular i/o
//   usage: write0 ttyname

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s ttyname\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int tty_fd = open(argv[1], O_WRONLY);
  if (tty_fd == -1) {
    fprintf(stderr, "Can not open \n");
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }

  char buf[BUFSIZ];
  int size;
  while ((size = read(1, buf, BUFSIZ)) > 0) {
    if (size == -1) {
      fprintf(stderr, "Can not read from stdin\n");
      exit(EXIT_FAILURE);
    }
    if (size != write(tty_fd, buf, size)) {
      fprintf(stderr, "Can not write to %s\n", argv[1]);
      exit(EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}
