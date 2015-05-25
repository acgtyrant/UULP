// 实践证明，Linux 不支持

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  int p[2];
  pipe(p);
  if (write(p[0], "hello", 5) == 0)
    printf("success\n");
  return EXIT_SUCCESS;
}
