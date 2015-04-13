// 书上说 _exit() 包括的操作是：
// ……
// (3) 如果父进程条用 wait 或 waitpid 来等待子进程结束，则通知父进程。
// (4) 向父进程发送 SIGCHLD.
//
// 我不禁困惑了，我以为父进程是通过 SIGCHLD 信号来调用 wait/waitpid,
// 但并非如此。POSIX.1-2001 默认父进程对 SIGCHLD 的处理是 SIGIGN.
// 所以实际上是父进程要主动且提前调用 wait/waitpid.
// 此外就算父进程没及时调用 wait/waitpid, 子进程退出时也会变成 Zombie
// 进程，保留了进程 ID，进程的终止状态，以及该进程使用的 CPU
// 时间等等，父进程照样可以调用 wait/waitpid 以接收这些信息。
// 所以，本编程练习中，我们可以直接在对 SIGCHLD 的处理中调用 wait/waitpid.
//
// 参考：
// 1. http://www.cnblogs.com/wuchanming/p/4020463.html
// 2. http://www.douban.com/note/156155337
//
// PS: 不知道怎么中断循环，干脆就在 parent_code 调用 exit 得了。

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define DELAY 5

extern void child_code(int delay);
extern void parent_code(int child_pid);

int main(void) {
  int newpid;
  printf("Before: my pid is %d\n", getpid());
  if ((newpid = fork()) == -1)
    perror("fork");
  else if (newpid == 0)
    child_code(DELAY);
  else {
    signal(SIGCHLD, parent_code);
    while (true) {
      printf("waiting\n");
      sleep(1);
    }
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
  int child_status;
  int wait_rv = wait(&child_status);

  printf("done waiting for %d, wait returned %d\n", child_pid, wait_rv);

  int high_8 = child_status >> 8;
  int low_7 = child_status & 0x7F;
  int bit_7 = child_status & 0x80;
  printf("high_8: %d, low_7: %d, bit_7: %d\n", high_8, low_7, bit_7);
  exit(EXIT_SUCCESS);
}
