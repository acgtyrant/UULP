// rotate.c: map a->b, b->c, .. z->a
//  purpose: useful for showing tty modes

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <termios.h>

extern void tty_mode(int how);
extern void set_cr_noecho_mode(void);
extern void signal_handler(int signum);

int main(void) {
  tty_mode(0); // save tty mode
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGTERM, signal_handler);
  set_cr_noecho_mode();
  int c;
  while ((c = getchar()) != EOF) {
    if (c == 'z') {
      c = 'a';
    }
    else {
      if (c == 'Q') {
        tty_mode(1);
        exit(EXIT_SUCCESS);
      }
      else {
        ++c;
      }
    }
    putchar(c);
  }
  tty_mode(1);
  return EXIT_SUCCESS;
}

extern void tty_mode(int how) {
  static struct termios original_mode;
  if (how == 0) {
    tcgetattr(0, &original_mode);
  }
  else {
    tcsetattr(0, TCSANOW, &original_mode);
  }
}

extern void set_cr_noecho_mode(void) {
  struct termios ttystate;
  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ICANON; // no buffering
  ttystate.c_lflag &= ~ECHO; // no echo either
  ttystate.c_cc[VMIN] = 1; // get one char at a time
  tcsetattr(0, TCSANOW, &ttystate);
}

extern void signal_handler(int signum) {
  tty_mode(1);
  exit(2);
}

