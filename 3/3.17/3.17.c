#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd = open("3.17.text", O_RDONLY);
  char buf[BUFSIZ * 2];
  read(fd, buf, BUFSIZ *2);
  printf("3.17.text: %s\n", buf);
  int sleep_time = 20;
  while (sleep_time) {
    printf("%d to sleep...\n", sleep_time);
    sleep(1);
    --sleep_time;
  }
  read(fd, buf, BUFSIZ * 2);
  printf("3.17.text: %s\n", buf);
  return EXIT_SUCCESS;
}
