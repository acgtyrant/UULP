#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h> // signal()
#include <unistd.h> // alarm(), pause()


//#define SHHHH

extern void wakeup(int);

int main(void) {
  printf("about to sleep for 4 seconds\n");
  signal(SIGALRM, wakeup);
  alarm(4);
  pause();
  printf("Morning so soon? \n");
  return EXIT_SUCCESS;
}

extern void wakeup(int signum) {
#ifndef SHHHH
  printf("Alarm received from kernel\n");
#endif
}
