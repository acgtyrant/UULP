#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  char buf[101]; buf[100] = '\0';
  int fd1 = open("2.9.text", O_RDONLY);
  lseek(fd1, 100, SEEK_END);

  if (read(fd1, buf, 100) == 100) {
    printf("Read it successfully from 100 bytes away of EOF, no error happend.\n");
  }
  else {
    fprintf(stderr, "Boooooooom! You failed to read it from 100 bytes away of EOF.\n");
    perror("2.9.text");
  }
  close(fd1);

  int fd2 = open("2.9.text", O_WRONLY);
  lseek(fd2, 100, SEEK_END);

  if (write(fd2, "hello", 5) == 5) {
    printf("Write it successfully from 100 bytes away of EOF, no error happend.\n");
  }
  else {
    fprintf(stderr, "Boooooooom! You failed to write it from 100 bytes away of EOF.\n");
    perror("2.9.text");
  }
  close(fd2);

  int fd3 = open("2.9.text", O_WRONLY);
  lseek(fd3, 20000, SEEK_END);

  if (write(fd3, "hello", 5) == 5) {
    printf("Write it successfully from 20000 bytes away of EOF, no error happend.\n");
  }
  else {
    fprintf(stderr, "Boooooooom! You failed to write it from 20000 bytes away of EOF.\n");
    perror("2.9.text");
  }
  close(fd3);

  return 0;
}
