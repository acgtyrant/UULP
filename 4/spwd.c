// spwd.c: a simplified version of pwd
//
// starts in current directory and recursively
// climbs up to root of filesystem, prints top part
// then prints current part
//
// uses readdir() to get info about each thing

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

extern ino_t get_ino(char *dir);
extern void printpath(ino_t this_inode);
extern void inode_num_to_name(ino_t this_ino, char this_inode_name[BUFSIZ]);

int main(int argc, char *argv[]) {
  int ino = get_ino(".");
  if (ino == 2) {
    printf("/\n");
    return EXIT_SUCCESS;
  }
  printpath(ino);
  printf("\n");
  return EXIT_SUCCESS;
}

// prints path leadin down to an object with this inode
// kindof recursive

extern void printpath(ino_t this_ino) {
  char this_inode_name[BUFSIZ];
  ino_t father_inode_num = get_ino("..");
  if (father_inode_num != this_ino) {
    inode_num_to_name(this_ino, this_inode_name);
    chdir("..");
    printpath(father_inode_num);
    printf("/%s", this_inode_name);
  }
}

// returns inode number of the file
extern ino_t get_ino(char *dir) {
  struct stat info;
  if (stat(dir, &info) == -1) {
    fprintf(stderr, "Cannot stat ");
    perror(dir);
    exit(EXIT_FAILURE);
  }
  return info.st_ino;
}

// looks through current directory for a file with this inode
// number and copies its name into namebuf

extern void inode_num_to_name(ino_t this_ino, char this_inode_name[]) {
  struct dirent *dirent_p;
  DIR *dir_p = opendir("..");
  if (dir_p == NULL) {
    perror("..");
    exit(EXIT_FAILURE);
  }
  // search directory for a file with specified inum
  while ((dirent_p = readdir(dir_p)) != NULL) {
    if (dirent_p->d_ino == this_ino) {
      strcpy(this_inode_name, dirent_p->d_name);
      closedir(dir_p);
      return ;
    }
  }
  fprintf(stderr, "error looking from inum %ld\n", this_ino);
  exit(EXIT_FAILURE);
}
