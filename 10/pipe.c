#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "usage: pipe cmd1 cmd2\n");
    exit(EXIT_FAILURE);
  }

  int pipefd[2];
  pipe(pipefd); // pipe 一定要在 fork 之前执行……
  int pid = fork();

  if (pid == -1) {
    fprintf(stderr, "fork failed\n");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], 1);
    close(pipefd[1]); // 强力关掉无用的管道 fd 是好习惯
    execlp(argv[1], argv[1], NULL);
    fprintf(stderr, "exec cmd 1 failed\n");
    exit(EXIT_FAILURE);
  }
  else {
    close(pipefd[1]);
    dup2(pipefd[0], 0);
    close(pipefd[0]); // 强力关掉无用的管道 fd 是好习惯
    execlp(argv[2], argv[2], NULL);
    fprintf(stderr, "exec cmd 2 failed\n");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
