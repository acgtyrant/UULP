#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>

int main(void) {
  initscr();
  clear();
  for (int i = 0; i < LINES; ++i) {
    move(i, i + i);
    if (i % 2 == 1)
      standout();
    // 本来想用 "(´∀`)σ)Д`)UCCU~" 的，但 curses 好像不支持 Unicode.
    addstr("Hello, world");
    if (i % 2 == 1)
      standend();
    refresh();
    sleep(1);
  }
  endwin();
  return EXIT_SUCCESS;
}
