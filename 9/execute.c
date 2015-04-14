#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

extern int execute(char **arglist) {
  if (arglist[0] == NULL)
    return 0;
  int pid = fork();
  int child_status = -1; // The return value, default set to -1 which show error.

  if (pid == -1) {
    perror("fork");
    return child_status;
  }
  else if (pid == 0) {
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    execvp(arglist[0], arglist);
    perror("cannot execute command");
    exit(EXIT_FAILURE);
  }
  else {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    if (wait(&child_status) == -1)
      perror("wait");
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    return child_status;
  }
}
