#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h> // setitimer()
#include <signal.h> // signal()
#include <unistd.h> // pause()
#include <stdbool.h>

extern void countdown(int);
extern int set_ticker(int n_msecs);

int main(void) {
  signal(SIGALRM, countdown);

  if (set_ticker(500) == -1) {
    perror("set_ticker");
    exit(EXIT_FAILURE);
  }
  else {
    while (true)
      pause();
  }
  return EXIT_SUCCESS;
}

extern void countdown(int signum) {
  static int num = 10;
  printf("%d..\n", num);
  num--;
  if (num < 0) {
    printf("Done!\n");
    exit(EXIT_SUCCESS);
  }
}

extern int set_ticker(int n_msecs) {
  struct itimerval new_timeset;
  long n_sec, n_usecs;

  n_sec = n_msecs / 1000;
  n_usecs = (n_msecs % 1000) * 1000L;

  new_timeset.it_value.tv_sec = n_sec;
  new_timeset.it_value.tv_usec = n_usecs;

  new_timeset.it_interval.tv_sec = n_sec;
  new_timeset.it_interval.tv_usec = n_usecs;

  return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
