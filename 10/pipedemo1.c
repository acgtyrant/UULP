#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
  int len, apipe[2];
  char buf[BUFSIZ];

  if (pipe(apipe) == -1) {
    perror("create a pipe failed");
    exit(EXIT_FAILURE);
  }
  printf("Got a pipe! It is file descriptors: {%d %d}\n", apipe[0], apipe[1]);

  while (fgets(buf, BUFSIZ, stdin)) {
    len = strlen(buf);

    if (write(apipe[1], buf, len) != len) {
      perror("write to a pipe failed");
      break;
    }

    for (int i = 0; i < len; ++i) {
      buf[i] = 'x';
    }

    len = read(apipe[0], buf, len);
    if (len == -1) {
      perror("read from a pipe failed");
      break;
    }

    if (write(1, buf, len) != len) {
      perror("write to stdout failed");
      break;
    }
  }
  return EXIT_SUCCESS;
}
