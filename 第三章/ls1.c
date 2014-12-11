// ls1.c
// purpose list contents of directory or directories
// action if no args, use . else list files in args

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

extern void do_ls(char *dirname);

int main(int argc, char *argv[]) {
  if (argc == 1) {
    do_ls(".");
  }
  else {
    while (--argc) {
      printf("%s:\n", *++argv);
      do_ls(*argv);
    }
  }
  return EXIT_SUCCESS;
}

extern void do_ls(char *dirname) {
  struct dirent *dirent_p;
  DIR *dir_p = opendir(dirname);
  if (dir_p == NULL) {
    perror(dirname);
    exit(EXIT_FAILURE);
  }
  while ((dirent_p = readdir(dir_p)) != NULL) {
    printf("%s\n", dirent_p->d_name);
  }
  closedir(dir_p);
  return ;
}
