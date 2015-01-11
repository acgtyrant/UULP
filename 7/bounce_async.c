// 教学版有问题，我这里的 pause 永远阻塞住了，并不能接收 SIGIO 和 SIG_ALRM

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // pause()
#include <fcntl.h> // fcntl()
#include <signal.h> // signal()
#include <sys/time.h> // set_ticker()
#include <stdbool.h>
#include <string.h>
#include "curses.h" // initscr(), cbreak(), getch(), move(), addstr(), mvaddch(), refresh(), endwin()

#define MESSAGE "hello"
#define BLANK "     "

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
bool done = false;

extern void set_ticker(int n_msecs);
extern void on_input(int signum);
extern void enable_kbd_signal(void);
extern void on_alarm(int signum);

int main(void) {
  initscr();
  cbreak();
  noecho();
  refresh();

  signal(SIGIO, on_input);
  enable_kbd_signal();
  signal(SIGALRM, on_alarm);
  set_ticker(delay);

  mvaddstr(row, col, MESSAGE);

  while (done == false)
    pause();

  endwin();
  return EXIT_SUCCESS;
}

extern void set_ticker(int n_msecs) {
  long n_secs = n_msecs / 1000L;
  long n_usecs = (n_msecs % 1000L) * 1000L;

  struct itimerval new_timeset;

  new_timeset.it_value.tv_sec = n_secs;
  new_timeset.it_value.tv_usec = n_usecs;

  new_timeset.it_interval.tv_sec = n_secs;
  new_timeset.it_interval.tv_usec = n_usecs;

  setitimer(ITIMER_REAL, &new_timeset, NULL);
}

extern void on_input(int signum) {
  int c = getch();

  if (c == 'Q' || c == EOF)
    exit(EXIT_SUCCESS);
  else if (c == ' ')
    dir = -dir;
}

extern void enable_kbd_signal(void) {
  fcntl(0, F_SETOWN, getpid());
  int fd_flags = fcntl(0, F_GETFL);
  fcntl(0, F_SETFL, fd_flags | O_ASYNC);
}

extern void on_alarm(int signum) {
  signal(SIGALRM, on_alarm);
  mvaddstr(row, col, BLANK);
  col += dir;
  mvaddstr(row, col, MESSAGE);
  refresh();

  if (dir == -1 && col <= 0)
    dir = 1;
  else if (dir == 1 && col + strlen(MESSAGE) >= COLS)
    dir = -1;
}
