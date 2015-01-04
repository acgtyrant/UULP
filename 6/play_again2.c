// play_again2.c
// purpose: ask if user wants another transaction
//  method: set tty into char-by-char mode and no -echo mode
//          read char, return result
// returns: 0=>yes, 1=>no
//  better: time out if user walks away

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <stdbool.h>

#define QUESTION "Do you want another transaction"

extern void tty_mode(int how);
extern void set_cr_noecho_mode(void);
extern int get_response(char *);

int main(int argc, char *argv[]) {
  tty_mode(0); // save tty mode
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

extern void set_cr_noecho_mode(void) {
  struct termios ttystate;
  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ICANON; // no buffering
  ttystate.c_lflag &= ~ECHO; // no echo either
  ttystate.c_cc[VMIN] = 1; // get one char at a time
  tcsetattr(0, TCSANOW, &ttystate);
}

extern int get_response(char *question) {
  printf("%s (y/n)?", question);
  while (true) {
    char c = getchar();
    switch(c) {
      case 'y':
      case 'Y':
        putchar('y');
        return 0;
      case 'n':
      case 'N':
      case EOF:
        putchar('n');
        return 1;
    }
  }
}
