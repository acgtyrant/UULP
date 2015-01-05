#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#define INPUTLEN 100

extern void sig_handler(int );

int main(void) {
  struct sigaction newhandler;
  sigset_t block;
  char input[INPUTLEN];
  newhandler.sa_handler = sig_handler;
  newhandler.sa_flags = SA_RESETHAND | SA_RESTART;

  sigemptyset(&block);
  sigaddset(&block, SIGQUIT);
  newhandler.sa_mask = block;

  if (sigaction(SIGINT, &newhandler, NULL) == -1) {
    perror("sigaction");
  }
  else {
    while (true) {
      fgets(input, INPUTLEN, stdin);
      printf("input: %s\n", input);
    }
  }

  return EXIT_SUCCESS;
}

extern void sig_handler(int signum) {
  printf("Called with signal %d\n", signum);
  sleep(signum);
  printf("done handling signal %d\n", signum);
}
