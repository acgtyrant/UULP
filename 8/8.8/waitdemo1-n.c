// 我发现 random() 是伪随机……也就是说 fork () 出的每一个子进程 random () 输出
// 全一样，得主动用时间来初始化 seed. 若要追求真随机，则用 /dev/random
// 为了避免时间雷同，用纳秒级别的函数即可：http://sodino.com/2015/03/20/c-time-micro-nano/
//
// 麻烦，算了，干脆用递增数列来初始化 seed 吧……

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

#define DELAY 10

extern void child_code(int parent_pid, int delay);
extern void parent_code(void);

int main(int argc, char *argv[]) {
  assert(argc == 2);
  int parent_pid = getpid();
  printf("Before: my pid is %d\n", parent_pid);
  for (int i = 0; i < atoi(argv[1]); ++i) {
    srandom(i);
    child_code(parent_pid, random() % DELAY + 1);
  }
  for (int i = 0; i < atoi(argv[1]); ++i) {
    parent_code();
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

extern void parent_code(void) {
  int wait_rv = wait(NULL);
  printf("parent: done waiting for %d\n", wait_rv);
}
