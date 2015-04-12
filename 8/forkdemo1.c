#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  printf("Before: my pid is %d\n", getpid());

  int ret_from_fork = fork();

  sleep(1);
  printf("After: my pid is %d\n, fork() said %d\n", getpid(), ret_from_fork);
  return EXIT_SUCCESS;
}
