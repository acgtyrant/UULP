// sigdemo2.c - shows how to ignore a signal
//            - press Ctrl-\ to kill this one

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

extern void f(int);

int main(int argc, char *argv[]) {
  signal(SIGINT, SIG_IGN);
  printf("You can not stop me!\n");
  while (true) {
    sleep(1);
    printf("UCCU (´∀`)σ)Д`)\n");
  }
  return EXIT_SUCCESS;
}
