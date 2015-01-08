#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <stdbool.h>
#include <curses.h>
#include "bounce.h"

struct pinball the_ball;

extern void set_up();
extern void ball_move(int );
extern void bounce_or_lose(struct pinball *);
extern void end_down();
extern void set_ticker(int );

int main(void) {
  set_up();

  int c;
  while ((c = getch()) != 'Q') {
    if (c == 'f') the_ball.x_ttm--;
    if (c == 's') the_ball.x_ttm++;
    if (c == 'F') the_ball.y_ttm--;
    if (c == 'S') the_ball.y_ttm++;
  }

  end_down();
  return EXIT_SUCCESS;
}

extern void set_up() {
  the_ball.x_pos = X_INIT_POS;
  the_ball.y_pos = Y_INIT_POS;
  the_ball.x_ttm = the_ball.x_ttg = X_TIM;
  the_ball.y_ttm = the_ball.y_ttg = Y_TIM;
  the_ball.x_dir = 1;
  the_ball.y_dir = 1;
  the_ball.symbol = DFL_SYMBOL;

  initscr();
  noecho();
  cbreak();

  signal(SIGALRM, SIG_IGN);
  mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
  refresh();

  signal(SIGALRM, ball_move);
  set_ticker(1000 / TICKS_PER_SEC);
}

extern void ball_move(int signum) {
  signal(SIGALRM, SIG_IGN);
  int x_org_pos = the_ball.x_pos,
      y_org_pos = the_ball.y_pos;
  bool moved = false;

  if (the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1) {
    the_ball.x_pos += the_ball.x_dir;
    the_ball.x_ttg = the_ball.x_ttm;
    moved = true;
  }
  if (the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1) {
    the_ball.y_pos += the_ball.y_dir;
    the_ball.y_ttg = the_ball.y_ttm;
    moved = true;
  }

  if (moved == true) {
    mvaddch(y_org_pos, x_org_pos, BLANK);
    mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
    bounce_or_lose(&the_ball);
    move(LINES - 1, COLS - 1);
    refresh();
  }

  signal(SIGALRM, ball_move);
}

extern void bounce_or_lose(struct pinball *the_ball_p) {
  if (the_ball_p->y_pos == TOP_EDGE)
    the_ball_p->y_dir = 1;
  if (the_ball_p->y_pos == BOTTOM_EDGE)
    the_ball_p->y_dir = -1;
  if (the_ball_p->x_pos == LEFT_EDGE)
    the_ball_p->x_dir = 1;
  if (the_ball_p->x_pos == RIGHT_EDGE)
    the_ball_p->x_dir = -1;
}

extern void end_down() {
  set_ticker(0);
  endwin();
}

extern void set_ticker(int n_msecs) {
  struct itimerval new_timeset;

  long n_sec = n_msecs / 1000;
  long n_usecs = (n_msecs % 1000) * 1000L;

  new_timeset.it_value.tv_sec = n_sec;
  new_timeset.it_value.tv_usec = n_usecs;

  new_timeset.it_interval.tv_sec = n_sec;
  new_timeset.it_interval.tv_usec = n_usecs;

  setitimer(ITIMER_REAL, &new_timeset, NULL);
}
