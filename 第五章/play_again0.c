// play_again0.c
// purpose: ask if user wants another transaction
//  method: ask a quesion, wait for yes/no anser
// returns: 0=>yes, 1=>no
//  better: eliminate need to press return

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <stdbool.h>

#define QUESTION "Do you want another transaction"

extern int get_response(char *);

int main(int argc, char *argv[]) {
  int response;
  response = get_response(QUESTION);
  return response;
}

extern int get_response(char *question) {
  int c;
  printf("%s (y/n)?", question);
  while (true) {
    switch(c = getchar()) {
      case 'y':
      case 'Y': return 0;
      case 'n':
      case 'N':
      case EOF: return 1;
      // no default, ignore all other input
    }
  }
}
