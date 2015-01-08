// crmode() 已改爲 cbreak(), 此外別忘了自己定義 set_ticker
// 此外 row, col, direction, delay, new_delay, c 等变量要声明为全局变量

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <curses.h>
#include <stdbool.h>
#include <string.h>

#define MESSAGE "hello"
#define BLANK   "     "

int row = 10, col = 0;
int direction = 1;
int delay = 200, new_delay = 0;
int c = 0;

extern int set_ticker(int );
extern void move_msg(int );

int main(void) {
  initscr();
  cbreak();
  noecho();

  move(row, col);
  addstr(MESSAGE);
  signal(SIGALRM, move_msg);
  set_ticker(delay);

  while (true) {
    new_delay = 0;
    c = getch();
    if (c == 'Q') break;
    if (c == ' ') direction *= -1;
    if (c == 'f' && delay > 2)
      new_delay = delay / 2;
    if (c == 's')
      new_delay = delay * 2;
    if (new_delay > 0)
      set_ticker(delay = new_delay);
  }

  endwin();
  return EXIT_SUCCESS;
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

extern void move_msg(int signum) {
  signal(SIGALRM, move_msg);

  move(row, col);
  addstr(BLANK);
  col += direction;
  move(row, col);
  addstr(MESSAGE);
  refresh();

  if (direction == -1 && col <= 0)
    direction = 1;
  if (direction == 1 && col + strlen(MESSAGE) >= COLS)
    direction = -1;
}
