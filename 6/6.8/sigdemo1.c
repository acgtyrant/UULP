//  sigdemo1.c - show how a signal handler works.
//  - run this and press Ctrl-C a few times
// 我曾在 f() 用 fflush(stdin), 以便抛弃接收一个字符后所剩下的缓冲，但实际上
// fflush(stdin) 是未定义的，参见
// http://segmentfault.com/q/1010000002462258/a-1020000002462281
// 现在改用 getchar() 优雅地抛弃缓冲～

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

extern void f(int);

int main(void) {
  signal(SIGINT, f);
  for (int i = 0; i < 5; ++i) {
    printf("blah blah blah\n");
    sleep(1);
  }
  return EXIT_SUCCESS;
}

extern void f(int signum) {
  printf("\tInterrupted! OK to quit (y/n)?");
  int c = getchar();
  if (c == EOF || c == 'y') {
    exit(EXIT_SUCCESS);
  }
  while (getchar() != '\n')
    ;
}
