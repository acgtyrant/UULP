// cp1.c
// version 1 of cp - uses read and write with tunable buffer size
//
// usage: cp1 src dest

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

extern void oops(const char *, const char *);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s source destination\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int src_fd = open(argv[1], O_RDONLY);
  if (src_fd == -1) {
    oops("Can not open", argv[1]);
  }

  struct stat stat_buf;
  if (stat(argv[2], &stat_buf) == -1) {
    oops("Failed to read stat", argv[2]);
  }

  // 连续用两次 strdup(), 第一次用 strdup() 产生的新 argv[2] 数组又不方便 free
  // 掉，此代码废弃，以此为戒。
  //if (S_ISDIR(buf.st_mode)) {
    //int len = strlen(argv[2]);
    //if (argv[2][len - 1] == '/') strdup(argv[2], "/");

    //strdup(argv[2], argv[1]);
  //}
  
  if (S_ISDIR(stat_buf.st_mode)) {
    int src_len = strlen(argv[1]);
    int dst_len = strlen(argv[2]);
    if (argv[2][dst_len - 1] != '/') {
      argv[2] = strndup(argv[2], dst_len + 1 + src_len);
      strcat(argv[2], "/");
    }
    else {
      argv[2] = strndup(argv[2], dst_len + src_len);
    }
    strcat(argv[2], argv[1]);
  }
  printf("argv[2]: %s\n", argv[2]);

  int dst_fd = creat(argv[2], COPYMODE);
  if (dst_fd == -1) {
    oops("Can not create", argv[2]);
  }

  char buf[BUFFERSIZE];
  int buf_size;

  while ((buf_size = read(src_fd, buf, BUFFERSIZE)) > 0)
    if (write(dst_fd, buf, buf_size) != buf_size)
      oops("Failed to create", argv[2]);

  if (buf_size == -1)
    oops("Failed to read", argv[1]);

  if (close(src_fd) == -1 || close(dst_fd) == -1)
    oops("Faile to close files.", "");

  free(argv[2]);
  return EXIT_SUCCESS;
}

extern void oops(const char *s1, const char *s2) {
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(EXIT_FAILURE);
}
