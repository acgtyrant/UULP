#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  printf("my pid is %d\n", getpid());
  int fork_rv = fork();

  if (fork_rv == -1)
    perror("fork");
  else if (fork_rv == 0)
    printf("I am the child, my pid = %d\n", getpid());
  else
    printf("I am the parent, my pid = %d\n", getpid());

  return EXIT_SUCCESS;
}
