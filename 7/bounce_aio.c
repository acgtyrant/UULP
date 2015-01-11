// 完全没明白 aio 机制……

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // pause()
#include <fcntl.h> // fcntl()
#include <signal.h> // signal()
#include <sys/time.h> // set_ticker()
#include <stdbool.h>
#include <string.h>
#include <aio.h>
#include "curses.h" // initscr(), cbreak(), getch(), move(), addstr(), mvaddch(), refresh(), endwin()

#define MESSAGE "hello"
#define BLANK "     "

struct aiocb kbcbuf;

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
bool done = false;

extern void set_ticker(int n_msecs);
extern void on_input(int signum);
extern void setup_aio_buffer();
extern void on_alarm(int signum);

int main(void) {
  initscr();
  cbreak();
  noecho();
  refresh();

  signal(SIGIO, on_input);
  setup_aio_buffer();
  aio_read(&kbcbuf);

  signal(SIGALRM, on_alarm);
  set_ticker(delay);

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
  int c;
  char *cp = (char *)kbcbuf.aio_buf;

  if (aio_error(&kbcbuf) != 0) {
    perror("reading failed");
  }
  else {
    if (aio_return(&kbcbuf) == 1) {
      c = *cp;
      if (c == 'Q' || c == EOF)
        exit(EXIT_SUCCESS);
      else if (c == ' ')
        dir = -dir;
    }
  }

  aio_read(&kbcbuf);
}

extern void setup_aio_buffer() {
  static char input[1];

  kbcbuf.aio_fildes = 0;
  kbcbuf.aio_buf = input;
  kbcbuf.aio_nbytes = 1;
  kbcbuf.aio_offset = 0;

  kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  kbcbuf.aio_sigevent.sigev_signo = SIGIO;
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
