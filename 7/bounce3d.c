// 增加了挡板机制。出于简化需要，直接固定在处于右边界且长度为五。
// Ball 撞到 bar 上就会留下空格，需要移动 bar 几下才会重新刷新出应有的
// BAR_SYMBOL, 不过懒得 Fix 了, 也没画边界

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <stdbool.h>
#include <unistd.h>
#include <curses.h>
#include "bounce.h"
#include "bar.h"

#define BARLEN 5

struct pinball the_ball;
// 为了简化问题，假定挡板与右边界重合
struct bar the_bar;

extern void set_up();
extern void b_move(int ); // 原本的 move 现在要同时处理 ball 和 bar 的移动
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
    
    if (c == 'j' && the_bar.bottom_y != BOTTOM_EDGE) 
      the_bar.dir = BAR_DOWN;
    if (c == 'k' && the_bar.top_y != TOP_EDGE)
      the_bar.dir = BAR_UP;
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
  the_ball.symbol = BALL_SYMBOL;

  the_bar.top_y = BAR_TOP_Y;
  the_bar.bottom_y = BAR_BOTTOM_Y;
  the_bar.dir = BAR_STOP;

  initscr();
  noecho();
  cbreak();

  signal(SIGALRM, SIG_IGN);
  mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
  for (int i = the_bar.top_y; i <= the_bar.bottom_y; ++i)
    mvaddch(i, RIGHT_EDGE, BAR_SYMBOL);
  refresh();

  signal(SIGALRM, b_move);
  set_ticker(1000 / TICKS_PER_SEC);
}

extern void b_move(int signum) {
  signal(SIGALRM, SIG_IGN);
  int x_org_pos = the_ball.x_pos,
      y_org_pos = the_ball.y_pos;
  bool ball_moved = false;

  if (the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1) {
    the_ball.x_pos += the_ball.x_dir;
    the_ball.x_ttg = the_ball.x_ttm;
    ball_moved = true;
  }
  if (the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1) {
    the_ball.y_pos += the_ball.y_dir;
    the_ball.y_ttg = the_ball.y_ttm;
    ball_moved = true;
  }

  if (ball_moved == true) {
    mvaddch(y_org_pos, x_org_pos, BLANK);
    mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
    bounce_or_lose(&the_ball);
    move(LINES - 1, COLS - 1);
  }
  if (the_bar.dir == BAR_UP && the_bar.top_y != TOP_EDGE) {
    mvaddch(the_bar.top_y + the_bar.dir, RIGHT_EDGE, BAR_SYMBOL);
    mvaddch(the_bar.bottom_y, RIGHT_EDGE, BLANK);
    the_bar.top_y += the_bar.dir;
    the_bar.bottom_y += the_bar.dir;
  }
  if (the_bar.dir == BAR_DOWN && the_bar.bottom_y != BOTTOM_EDGE) {
    mvaddch(the_bar.top_y, RIGHT_EDGE, BLANK);
    mvaddch(the_bar.bottom_y + the_bar.dir, RIGHT_EDGE, BAR_SYMBOL);
    the_bar.top_y += the_bar.dir;
    the_bar.bottom_y += the_bar.dir;
  }

  if (ball_moved || the_bar.dir != BAR_STOP) {
    refresh();
    the_bar.dir = BAR_STOP;
  }

  signal(SIGALRM, b_move);
}

extern void bounce_or_lose(struct pinball *the_ball_p) {
  if (the_ball_p->y_pos == TOP_EDGE)
    the_ball_p->y_dir = 1;
  if (the_ball_p->y_pos == BOTTOM_EDGE)
    the_ball_p->y_dir = -1;
  if (the_ball_p->x_pos == LEFT_EDGE)
    the_ball_p->x_dir = 1;

  if (the_ball_p->x_pos == RIGHT_EDGE) {
    if (the_ball_p->y_pos >= the_bar.top_y &&
        the_ball.y_pos <= the_bar.bottom_y) {
          mvaddch(the_ball.y_pos, the_ball.x_pos, BAR_SYMBOL);
          the_ball_p->x_dir = -1;
    }
    else {
      move(0, 0);
      addstr("You lost!");
      refresh();
      sleep(3);
      end_down();
      exit(EXIT_SUCCESS);
    }
  }
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
