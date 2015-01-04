#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  mkdir("demodlr", 0744);
  creat("y", 0744);
  chdir("demodlr");
  mkdir("a", 0744);
  mkdir("c", 0744);
  chdir("a");
  creat("x", 0744);
  chdir("../c");
  mkdir("d1", 0744);
  mkdir("d2", 0744);
  chdir("d1");
  creat("xlink", 0744);
  chdir("../d2");
  creat("xcopy", 0744);
  chdir("..");
  chdir("..");
  return EXIT_SUCCESS;
}
