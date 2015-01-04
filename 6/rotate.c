// rotate.c: map a->b, b->c, .. z->a
//  purpose: useful for showing tty modes

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
  int c;
  while ((c = getchar()) != EOF) {
    if (c == 'z') {
      c = 'a';
    }
    else {
      if (islower(c)){
        ++c;
      }
    }
    putchar(c);
  }
  return EXIT_SUCCESS;
}
