// filesize.c - prints size of passwd file

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  struct stat buf;

  //stat() 读取成功时返回 0, 否则返回 -1
  if (stat("/etc/passwd", &buf) == 0) {
    printf("The size of /etc/passwd is %d\n", buf.st_size);
  }
  else {
    perror("/etc/passwd");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
