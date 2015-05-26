#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lclnt_funcs1.h"

extern void do_regular_work(void);

int main(void) {
  setup();
  if (get_ticket() != 0)
    exit(EXIT_FAILURE);
  do_regular_work();
  release_ticket();
  shut_down();
  return EXIT_SUCCESS;
}

extern void do_regular_work(void) {
  printf("SuperSleep version 1.0 Running - Licensed Software\n");
  sleep(10);
}
