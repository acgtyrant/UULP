#include <stdio.h>
#include <stdlib.h>
#include <curses.h> // initscr(), clear(), standout(), standend(), refresh(),
                    // addstr(), move(), endwin()
#include <stdbool.h> // true
#include <unistd.h> // sleep()

#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW 10

int main(void) {
  char message[] = "hello";
  char blank[] = "     ";

  initscr();
  clear();
  int dir = 1;
  int pos = LEFTEDGE;
  while (true) {
    move(ROW, pos);
    addstr(message);
    move(LINES - 1, COLS - 1);
    refresh();
    sleep(1);
    move(ROW, pos);
    addstr(blank);
    pos += dir;
    if (pos >= RIGHTEDGE)
      dir = -1;
    if (pos <= LEFTEDGE)
      dir = 1;
  }
  return EXIT_SUCCESS;
}
