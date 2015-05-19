#include <stdio.h>
#include <stdlib.h>

int main(void) {
  FILE *fp = popen("mail admin backup", "w");

  fprintf(fp, "Error with backup!\n");
  pclose(fp);
  return EXIT_SUCCESS;
}
