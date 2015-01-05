#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define INPUTLEN 100

extern void int_handler(int );
extern void quit_handler(int );

int main(void) {
  signal(SIGINT, int_handler);
  signal(SIGQUIT, quit_handler);

  int nchars;
  char input[INPUTLEN];
  do {
    printf("\nType a message\n");
    nchars = read(0, input, (INPUTLEN - 1));
    if (nchars == -1)
      perror("read returned an error");
    else {
      input[nchars] = '\0';
      printf("You typed: %s\n", input);
    }
  } while (strncmp(input, "quit", 4) != 0);

  return EXIT_SUCCESS;
}

extern void int_handler(int signum) {
  printf("Received signal %d .. wating\n", signum);
  sleep(2);
  printf("Leaving int_handler\n");
}

extern void quit_handler(int signum) {
  printf("Received signal %d .. waiting\n", signum);
  sleep(3);
  printf("Leaving quit_handler\n");
}
