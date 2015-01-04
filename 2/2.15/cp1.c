// cp1.c
// version 1 of cp - uses read and write with tunable buffer size
//
// usage: cp1 src dest

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

extern void oops(const char *, const char *);

int main(int argc, char *argv[]) {
  // 接收 -i 参数并处理，TODO: 嵌套太多，有待优化
  bool prompt_mode = false;
  if (argc == 4) {
    int i = 1;
    while (i < argc) {
      if (strcmp(argv[i], "-i") == 0) {
        prompt_mode = true;
        if (i != 3) argv[i] = argv[i + 1];
        --argc;
        break;
      }
      ++i;
    }
  }

  if (argc != 3) {
    fprintf(stderr, "Usage: %s source destination\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int src_fd = open(argv[1], O_RDONLY);
  if (src_fd == -1) {
    oops("Can not open", argv[1]);
  }

  // 当有 -i 参数且目标文件存在时，进一步接收交互输入
  if (open(argv[2], O_RDONLY) != -1 && prompt_mode) {
    printf("%s: Do you replace \"%s\"?", argv[0], argv[2]);
    char response[BUFSIZ];
    scanf("%s", response);
    if (strcmp(response, "y") != 0 && strcmp(response, "yes") != 0) {
      exit(EXIT_SUCCESS);
    }
  }

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

  return EXIT_SUCCESS;
}

extern void oops(const char *s1, const char *s2) {
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(EXIT_FAILURE);
}
