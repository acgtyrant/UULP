#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

extern bool isaregfile(int fd) {
  struct stat info;
  fstat(fd, &info);
  if (info.st_mode == S_IFREG)
    return true;
  return false;
}
