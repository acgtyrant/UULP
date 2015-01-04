//  sigdemo1.c - show how a signal handler works.
//  - run this and press Ctrl-C a few times

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

extern void f(int);

int main(int argc, char *argv[]) {
  signal(SIGINT, f);
  for (int i = 0; i < 5; ++i) {
    printf("blah blah blah\n");
    sleep(1);
  }
  return EXIT_SUCCESS;
}

extern void f(int signum) {
  printf("Shut up.\n");
}
