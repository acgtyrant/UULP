#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

FILE *popen(const char *command, const char *mode) {
  int p[2];
  int parent_end, child_end;

  if (*mode == 'r') {
    parent_end = READ;
    child_end = WRITE;
  }
  else if (*mode == 'w') {
    parent_end = WRITE;
    child_end = READ;
  }
  pipe(p);

  if (fork() > 0) {
    close(p[child_end]);
    return fdopen(p[parent_end], mode);
  }

  close(p[parent_end]);
  dup2(p[child_end], child_end);
  close(p[child_end]);

  execl("/bin/sh", "sh", "-c", command, NULL);
  return EXIT_SUCCESS;
}
