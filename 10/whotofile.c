#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void) {
  printf("About to run who into a file\n");

  int pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    close(0);
    creat("userlist", 0644);
    execlp("who", "who", NULL);
  }

  if (pid != 0) {
    wait(NULL);
    printf("Done running who. results in userlist\n");
  }

  return EXIT_SUCCESS;
}
