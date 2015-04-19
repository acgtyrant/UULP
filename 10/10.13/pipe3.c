// 代码相当冗余，希望将来能学到怎么优化

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "usage: pipe cmd1 cmd2 cmd3\n");
    exit(EXIT_FAILURE);
  }

  int pipefd[2];
  pipe(pipefd); // pipe 一定要在 fork 之前执行……
  int pid = fork();

  if (pid == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], 1);
    close(pipefd[1]); // 强力关掉无用的管道 fd 是好习惯
    execlp(argv[1], argv[1], NULL);
  }
  else {
    close(pipefd[1]);
    dup2(pipefd[0], 0);
    close(pipefd[0]); // 强力关掉无用的管道 fd 是好习惯
    pipe(pipefd);
    pid = fork();
    if (pid == 0) {
      close(pipefd[0]);
      dup2(pipefd[1], 1);
      close(pipefd[1]);
      execlp(argv[2], argv[2], NULL);
    }
    else {
      close(pipefd[1]);
      dup2(pipefd[0], 0);
      close(pipefd[0]);
      execlp(argv[3], argv[3], NULL);
    }
  }

  return EXIT_SUCCESS;
}
