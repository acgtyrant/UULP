#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int pid = fork();

  if (pid == 0) {
    argv++;
    execvp(*argv, argv);
  }
  else {
    wait(NULL);
    char buf[BUFSIZ];
    while (fgets(buf, BUFSIZ, stdin) != NULL)
      printf("%s\n", buf);
  }
  return EXIT_SUCCESS;
}
