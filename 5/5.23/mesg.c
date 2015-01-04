// 经过实验，`mesg` 会根据选项修改 tty 设备在用户组上的修改权限。
// 只实现了最基本的功能，不考虑显示状态以及其它参数原生支持。

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 2 || strchr("yYnN", *argv[1]) == NULL) {
    printf("Usage: %s [y|n]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char respone = tolower(*argv[1]);
  char *tty_name = ttyname(0);
  struct stat tty;
  stat(tty_name, &tty);
  mode_t mode = tty.st_mode;
  
  if (respone == 'y') {
    chmod(tty_name, mode | S_IWGRP);
  }
  else {
    chmod(tty_name, mode & ~S_IWGRP);
  }

  return EXIT_SUCCESS;
}
