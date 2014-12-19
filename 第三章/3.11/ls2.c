// ls2.c
// purpose list contents of directory or directories
// action if no args, use . else list files in args
// note uses stat and pwd.h and grp.h
// BUG: try ls2 /tmp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include <unistd.h>

extern void do_ls(char *dirname);
extern void do_stat(char *filename);
extern void show_stat_info(char *filename, struct stat *buf_p);
extern void mode_to_letters(int mode, char *letters);
extern char *uid_to_name(uid_t);
extern char *gid_to_name(gid_t);

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

  if (chdir(dirname) == -1) {
    perror(dirname);
    exit(EXIT_FAILURE);
  }

  while ((dirent_p = readdir(dir_p)) != NULL) {
    do_stat(dirent_p->d_name);
  }
  closedir(dir_p);
  return ;
}

extern void do_stat(char *filename) {
  struct stat stat_buf;

  if ((stat(filename, &stat_buf) == -1)) {
    perror(filename);
    exit(EXIT_FAILURE);
  }
  else {
    show_stat_info(filename, &stat_buf);
  }
}

extern void show_stat_info(char *filename, struct stat *stat_buf_p) {
  char letters[11];
  mode_to_letters(stat_buf_p->st_mode, letters);
  printf("%s ", letters);

  printf("%ld ", stat_buf_p->st_nlink);

  char *uid_name = uid_to_name(stat_buf_p->st_uid);
  printf("%-10s ", uid_name);

  char *gid_name = gid_to_name(stat_buf_p->st_gid);
  printf("%-10s ", gid_name);

  printf("%ld ", stat_buf_p->st_size);

  char *time_str = ctime(&(stat_buf_p->st_mtime)) + 4;
  printf("%.12s ", time_str);

  printf("%s ", filename);
  printf("\n");
}

// utility functions

// This function takes a mode value and a char array
// and puts into the cahr array the file type and the
// nine letters that correspond to the bits in mode.
// NOTE: It does not code setuid, setgid, and sticky
// codes.

extern void mode_to_letters(int mode, char *letters) {
  strcpy(letters, "----------");

  if (S_ISREG(mode)) letters[0] = '-';
  if (S_ISDIR(mode)) letters[0] = 'd';
  if (S_ISLNK(mode)) letters[0] = 'l';
  if (S_ISCHR(mode)) letters[0] = 'c';
  if (S_ISBLK(mode)) letters[0] = 'b';
  //if (S_ISSOCK(mode)) letters[0] = 's';
  //由于不明原因，无法调用 S_ISSOCK() 宏
  if (S_ISFIFO(mode)) letters[0] = 'p';

  if (mode & S_IRUSR) letters[1] = 'r';
  if (mode & S_IWUSR) letters[2] = 'w';
  if (mode & S_IXUSR) letters[3] = 'x';

  if (mode & S_IRGRP) letters[4] = 'r';
  if (mode & S_IWGRP) letters[5] = 'w';
  if (mode & S_IXGRP) letters[6] = 'x';

  if (mode & S_IROTH) letters[7] = 'r';
  if (mode & S_IWOTH) letters[8] = 'w';
  if (mode & S_IXOTH) letters[9] = 'x';
}

extern char *uid_to_name(uid_t uid) {
  struct passwd *pwd = getpwuid(uid);
  static char str[10];

  if (pwd == NULL) {
    sprintf(str, "%d", uid);
    return str;
  }
  else {
    return pwd->pw_name;
  }
}

extern char *gid_to_name(gid_t gid) {
  struct group *grp = getgrgid(gid);
  static char str[10];

  if (grp == NULL) {
    sprintf(str, "%d", gid);
    return str;
  }
  else {
    return grp->gr_name;
  }
}
