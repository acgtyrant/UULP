// 我发现，可以要么父进程 fork 出子进程并等，子进程 fork 出子子进程并等，
// 要么父进程 fork 两个子进程并等。从题意来说，显然后者更好。

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DELAY 2

extern void child_code(int delay);
extern void parent_code(int child_pid);

int main(void) {
  int newpid_1, newpid_2;
  printf("Before: my pid is %d\n", getpid());
  if ((newpid_1 = fork()) == -1)
    perror("fork");
  else if (newpid_1 == 0)
    child_code(DELAY);
  else {
    if ((newpid_2 = fork()) == -1)
      perror("fork");
    else if (newpid_2 == 0)
      child_code(DELAY);
    parent_code(newpid_1);
    parent_code(newpid_2);
  }
  
  return EXIT_SUCCESS;
}

extern void child_code(int delay) {
  printf("child %d here, will sleep for %d seconds\n", getpid(), delay);
  sleep(delay);
  printf("child done, about to exit\n");
  exit(17);
}

extern void parent_code(int child_pid) {
  int wait_rv = wait(NULL);
  printf("done waiting for %d, wait returned %d\n", child_pid, wait_rv);
}
