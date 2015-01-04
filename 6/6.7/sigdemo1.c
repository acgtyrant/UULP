//  sigdemo1.c - show how a signal handler works.
//  - run this and press Ctrl-C a few times
//  心血来潮地没按题目来，改用颜文字玩。

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

extern void f(int);
extern void uccu();

size_t times = 0;
size_t times_limit = 3;

int main(int argc, char *argv[]) {
  if (argc > 1) {
    times_limit = *argv[1];
  }

  signal(SIGINT, f);
  for (int i = 0; i < 10; ++i) {
    printf("Pia!<(=ｏ ‵-′)ノ☆\n");
    sleep(1);
  }
  return EXIT_SUCCESS;
}

extern void f(int signum) {
  if (times == times_limit) {
    printf("<(=o゜▽゜)o☆[BINGO!]\n");
    exit(EXIT_SUCCESS);
  }

  uccu();
}

extern void uccu() {
  times++;
  size_t times_tmp = times;
  while (times_tmp != 0) {
    printf("UCCU (´∀`)σ)Д`)\n");
    --times_tmp;
  }
  putchar('\n');
}
