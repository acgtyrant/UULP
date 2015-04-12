#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  printf("my pid is %d\n", getpid());
  fork();
  fork();
  fork();
  printf("my pid is %d\n", getpid());
  return EXIT_SUCCESS;
}
