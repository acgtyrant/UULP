#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s username\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *username = argv[1];
  char *ttyname = NULL;

  struct utmp utmp_buf;
  int utmp_fd;
  int utmp_size = sizeof(struct utmp);

  if ((utmp_fd = open(UTMP_FILE, O_RDONLY)) == -1) {
    printf("Can not open\n");
    perror(UTMP_FILE);
    exit(EXIT_FAILURE);
  }

  while (read(utmp_fd, &utmp_buf, utmp_size) == utmp_size) {
    if (strcmp(username, utmp_buf.ut_user) == 0) {
      ttyname = utmp_buf.ut_line;
      break;
    }
  }

  close(utmp_fd);

  if (ttyname == NULL) {
    printf("%s may be not logged on or there is no such a user.\n", username);
    exit(EXIT_FAILURE);
  }
  else {
    printf("Welcome! You are chatting with %s from %s now.\n", username, ttyname);
  }

  char *origin_ttyname = ttyname;
  ttyname = strndup("/dev/", 10);
  strcat(ttyname, origin_ttyname);

  int tty_fd = open(ttyname, O_WRONLY);
  if (tty_fd == -1) {
    fprintf(stderr, "Can not open \n");
    perror(ttyname);
    exit(EXIT_FAILURE);
  }
  free(ttyname);

  char buf[BUFSIZ];
  int size;
  while ((size = read(1, buf, BUFSIZ)) > 0) {
    if (size == -1) {
      fprintf(stderr, "Can not read from stdin\n");
      exit(EXIT_FAILURE);
    }
    if (size != write(tty_fd, buf, size)) {
      fprintf(stderr, "Can not write to %s\n", argv[1]);
      exit(EXIT_FAILURE);
    }
  }

  close(tty_fd);
  return EXIT_SUCCESS;
}
