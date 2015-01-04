// 其实没必要用到那么多库，又加那么严苛的异常处理机制。为了高效，下次不再如此严格。

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int fd1 = open("2.6.text", O_RDONLY);
  int fd2 = open("2.6.text", O_WRONLY);
  int fd3 = open("2.6.text", O_RDONLY);

  char buf[21]; buf[20] = '\0';

  if (read(fd1, buf, 20) == 20) {
    printf("fd1: %s\n", buf);
  }
  close(fd1);

  strcpy(buf, "testing 123...");
  int len = strlen(buf);
  if (write(fd2, buf, len) != len) {
    perror("2.6.text");
    exit(EXIT_FAILURE);
  }
  close(fd2);

  if (read(fd3, buf, 20) == 20) {
    printf("fd1: %s\n", buf);
  }
  close(fd3);

  return EXIT_SUCCESS;
}
