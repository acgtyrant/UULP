#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

int main(void) {
  struct termios info;
  int respone = tcgetattr(0, &info);
  if (respone == -1) {
    perror("tcgetatter");
    exit(EXIT_FAILURE);
  }
  if (info.c_oflag & OLCUC)
    printf("OLCUC is on.\n");
  else
    printf("OLCUC is off.\n");
  
  return EXIT_SUCCESS;
}
