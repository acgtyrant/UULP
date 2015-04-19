#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define CLOSE_DUP

int main(void) {
  char line[100];

  fgets(line, 100, stdin); printf("%s", line);
  fgets(line, 100, stdin); printf("%s", line);
  fgets(line, 100, stdin); printf("%s", line);


  int fd = open("/etc/passwd", O_RDONLY);
  if (fd != 0) {
    fprintf(stderr, "Could not open data as fd 0\n");
    exit(EXIT_FAILURE);
  }

  int new_fd;
#ifdef CLOSE_DUP
  close(0);
  new_fd = dup(fd);
#else
  new_fd = dup2(fd, 0);
#endif

  if (new_fd != 0) {
    fprintf(stderr, "Could not open data as fd 0\n");
    exit(EXIT_FAILURE);
  }

  close(fd);

  fgets(line, 100, stdin); printf("%s", line);
  fgets(line, 100, stdin); printf("%s", line);
  fgets(line, 100, stdin); printf("%s", line);

  return EXIT_SUCCESS;
}
