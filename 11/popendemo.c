#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int i = 0;
  char buf[BUFSIZ];

  FILE *fp = popen("who | sort", "r");

  while (fgets(buf, BUFSIZ, fp) != NULL)
    printf("%3d %s", i++, buf);

  pclose(fp);
  return EXIT_SUCCESS;
}
