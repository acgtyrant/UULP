// play_again1.c
// purpose: ask if user wants another transaction
//  method: set tty into char-by-char mode, read char, return result
// returns: 0=>yes, 1=>no
//  better: do no echo inappropriate input

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <stdbool.h>

#define QUESTION "Do you want another transaction"

extern void tty_mode(int how);
extern void set_crmode(void);
extern int get_response(char *);

int main(int argc, char *argv[]) {
  tty_mode(0); // save tty mode
  set_crmode();
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

extern void set_crmode(void) {
  struct termios ttystate;
  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ICANON; // no buffering
  ttystate.c_cc[VMIN] = 1; // get one char at a time
  tcsetattr(0, TCSANOW, &ttystate);
}

extern int get_response(char *question) {
  printf("%s (y/n)?", question);
  while (true) {
    char c = getchar();
    switch(c) {
      case 'y':
      case 'Y': return 0;
      case 'n':
      case 'N':
      case EOF: return 1;
      default:
                printf("\ncannot understand %c, ", c);
                printf("Please type y or n\n");
    }
  }
}
