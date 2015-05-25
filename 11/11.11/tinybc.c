// 不知为啥，无法如预期工作……

#include <stdio.h> // fdopen()
#include <stdlib.h>
#include <unistd.h> // pipe()
#include <sys/wait.h>
#include <sys/socket.h>

extern void be_dc(int p[2]);
extern void be_bc(int p[2]);

int main(void) {
  int p[2];
  socketpair(AF_UNIX, SOCK_STREAM, PF_UNSPEC, p);
  
  if (fork() == 0) {
    be_dc(p);
  }
  else {
    be_bc(p);
    wait(NULL);
  }
  return EXIT_SUCCESS;
}

extern void be_dc(int p[2]) {
  dup2(p[0], 0);
  dup2(p[0], 1);
  close(p[0]);
  close(p[1]);

  execlp("dc", "dc", "-", NULL);
}

extern void be_bc(int p[2]) {
  FILE *fpin = fdopen(p[1], "w");
  FILE *fpout = fdopen(p[1], "r");
  close(p[0]);
  close(p[1]);

  int num1, num2;
  char operation[BUFSIZ], message[BUFSIZ];
  while (printf("tinybc: "), fgets(message, BUFSIZ, stdin) != NULL) {
    if (sscanf(message, "%d%[-+*/^]%d", &num1, operation, &num2) != 3) {
      printf("syntax error\n");
      continue;
    }
    fprintf(fpin, "%d\n%d\n%c\np\n", num1, num2, *operation);
    fflush(fpin);
    if (fgets(message, BUFSIZ, fpout) == NULL)
      break;
    printf("%d %c %d = %s", num1, *operation, num2, message);
  }
  
  fclose(fpin);
  fclose(fpout);
}
