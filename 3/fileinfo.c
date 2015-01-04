// fileinfo.c - use stat() to obtain and print file properties
//            - some menbers are just numbers

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

extern void show_stat_info(char *filename, struct stat *stat_buf);

int main(int argc, char *argv[]) {
  struct stat stat_buf;
  while (--argc > 0) {
    if (stat(*++argv, &stat_buf) == -1) {
      perror(*argv);
      return EXIT_FAILURE;
    }
    show_stat_info(*argv, &stat_buf);
  }
  return EXIT_SUCCESS;
}

extern void show_stat_info(char *filename, struct stat *stat_buf_p) {
  printf("  mode: %o\n", stat_buf_p->st_mode);
  printf(" links: %d\n", stat_buf_p->st_nlink);
  printf("  user: %d\n", stat_buf_p->st_uid);
  printf(" group: %d\n", stat_buf_p->st_gid);
  printf("  size: %d\n", stat_buf_p->st_size);
  printf(" mtime: %d\n", stat_buf_p->st_mtime);
  printf("  name: %s\n", filename);
  return ;
}
