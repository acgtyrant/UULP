// sigdemo2.c - shows how to ignore a signal
//            - press Ctrl-\ to kill this one

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <termios.h>

int main(void) {
  struct termios tc;
  tcgetattr(0, &tc);
  tc.c_lflag &= ~ISIG;
  tcsetattr(0, TCSANOW, &tc);
  printf("You can not stop me!\n");
  while (true) {
    sleep(1);
    printf("UCCU (´∀`)σ)Д`)\n");
  }
  tc.c_lflag |= ISIG;
  tcsetattr(0, TCSANOW, &tc);
  return EXIT_SUCCESS;
}
