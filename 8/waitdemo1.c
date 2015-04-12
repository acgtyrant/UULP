#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DELAY 2

extern void child_code(int delay);
extern void parent_code(int child_pid);

int main(void) {
  int newpid;
  printf("Before: my pid is %d\n", getpid());
  if ((newpid = fork()) == -1)
    perror("fork");
  else if (newpid == 0)
    child_code(DELAY);
  else
    parent_code(newpid);
  
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
