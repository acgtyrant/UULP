#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int main(void) {
  initscr();

  clear();
  move(10, 20);
  addstr("Hello, world");
  move(LINES - 1, 0);
  refresh();
  getch();
  endwin();
  return EXIT_SUCCESS;
}
