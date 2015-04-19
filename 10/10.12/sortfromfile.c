#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

int main(int argc, char *argv[]) {
  assert(argc == 2);
  close(0);
  open(argv[1], O_RDONLY);
  execlp("sort", "sort", NULL);
  return EXIT_SUCCESS;
}
