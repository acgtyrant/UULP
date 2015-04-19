#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  printf("Number of args: %d, Args are:\n", argc);
  for (int i = 0; i < argc; ++i)
    printf("argv[%d]: %s\n", i, argv[i]);
  fprintf(stderr, "This message is sent to stderr.\n");
  return EXIT_SUCCESS;
}
