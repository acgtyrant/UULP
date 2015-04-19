// 好吧刚完成 10.13, 就立马要求递归优化了……

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern void repipe(int n, char *argv[]);

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    fprintf(stderr, "cmds must be more than one!\n");
    exit(EXIT_FAILURE);
  }

  repipe(2, argv);
  return EXIT_SUCCESS;
}

// 写了似乎不怎么漂亮的递归函数……不过大概也不能优化成尾递归

extern void repipe(int n, char *argv[]) {
  int pipefd[2];
  pipe(pipefd); // pipe 一定要在 fork 之前执行……
  int pid = fork();
  if (pid == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], 1);
    close(pipefd[1]); // 强力关掉无用的管道 fd 是好习惯
    execlp(argv[n - 1], argv[n - 1], NULL);
  }
  else {
    close(pipefd[1]);
    dup2(pipefd[0], 0);
    close(pipefd[0]); // 强力关掉无用的管道 fd 是好习惯
    if (argv[n + 1] == NULL)
      execlp(argv[n], argv[n], NULL);
    else
      repipe(n + 1, argv);
  }
}
