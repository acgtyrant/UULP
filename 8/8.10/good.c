// 大概是因为执行得太快了，只开了两个子进程，信号处理就已经跟不上以致丢失了
// 子进程退出的消息。
//
// 不知为啥，按 http://www.cnblogs.com/wuchanming/p/4020463.html 并行不通。
// 算了留下这坑不管了。

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <stdbool.h>

#define DELAY 5

extern void child_code(int parent_pid, int delay);
extern void parent_code(int SIGNUM);

int process_num = 0; // 全局变量化，接收 argv[1] 参数，也方便 parent_code.
int tmp_process_num = 0; // parent_code 计数用。

int main(int argc, char *argv[]) {
  process_num = atoi(argv[1]);
  int parent_pid = getpid();
  printf("Before: my pid is %d\n", parent_pid);
  for (int i = 0; i < process_num; ++i)
    child_code(parent_pid, DELAY);
  signal(SIGCHLD, parent_code);
  while (true) {
    printf("waiting...\n");
    sleep(1);
  }
  return EXIT_SUCCESS;
}

extern void child_code(int parent_pid, int delay) {
  if (getpid() != parent_pid || fork() != 0)
    return ;
  printf("child %d here, will sleep for %d seconds\n", getpid(), delay);
  sleep(delay);
  printf("child: done, about to exit\n");
  exit(17);
}

extern void parent_code(int SIGNUM) {
  int child_status;
  int wait_rv;
  
  while ((wait_rv = waitpid(-1, &child_status, WNOHANG)) > 0) {
    printf("done waiting for %d\n", wait_rv);

    int high_8 = child_status >> 8;
    int low_7 = child_status & 0x7F;
    int bit_7 = child_status & 0x80;
    printf("high_8: %d, low_7: %d, bit_7: %d\n", high_8, low_7, bit_7);
    ++tmp_process_num;
    if (tmp_process_num == process_num)
      exit(EXIT_SUCCESS);
  }
}
