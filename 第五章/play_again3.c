// play_again3.c
// purpose: ask if user wants another transaction
//  method: set tty into char-by-char mode, no -echo mdoe
//          set tty into no -delay mode
//          read char, return result
// returns: 0=>yes, 1=>no, 2=>timeout
//  better: reset terminal mode on Interrupt

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define QUESTION "Do you want another transaction"
#define TRIES 3
#define SLEEPTIME 2
#define BEEP putchar('\a')

extern void tty_mode(int how);
extern void set_nodelay_mode(void);
extern void set_cr_noecho_mode(void);
extern int get_ok_char(void);
extern int get_response(char *);

int maxtries = TRIES;

int main(int argc, char *argv[]) {
  tty_mode(0); // save tty mode
  set_nodelay_mode();
  set_cr_noecho_mode();
  int response = get_response(QUESTION);
  tty_mode(1); // reload tty mode
  return response;
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

extern void set_nodelay_mode(void) {
  int termflags;
  termflags = fcntl(0, F_GETFL);
  termflags |= O_NDELAY;
  fcntl(0, F_SETFL, termflags);
}

extern void set_cr_noecho_mode(void) {
  struct termios ttystate;
  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ICANON; // no buffering
  ttystate.c_lflag &= ~ECHO; // no echo either
  ttystate.c_cc[VMIN] = 1; // get one char at a time
  tcsetattr(0, TCSANOW, &ttystate);
}

extern int get_ok_char(void) {
  int c;
  while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL) {
    ;
  }
  return c;
}

extern int get_response(char *question) {
  int c;
  printf("%s (y/n)?", question);
  fflush(stdout);
  while (true) {
    sleep(SLEEPTIME);
    c = tolower(get_ok_char());
    if (c != EOF)
      putchar(c);
    if (c == 'y')
      return 0;
    if (c == 'n')
      return 1;
    if (maxtries-- == 0)
      return 2;
    putchar('a');
    BEEP;
  }
}
