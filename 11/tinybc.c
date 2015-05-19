#include <stdio.h> // fdopen()
#include <stdlib.h>
#include <unistd.h> // pipe()
#include <sys/wait.h>

extern void be_dc(int in[2], int out[2]);
extern void be_bc(int in[2], int out[2]);

int main(void) {
  int todc[2], fromdc[2];
  pipe(todc);
  pipe(fromdc);
  
  if (fork() == 0) {
    be_dc(todc, fromdc);
  }
  else {
    be_bc(todc, fromdc);
    wait(NULL);
  }
  return EXIT_SUCCESS;
}

extern void be_dc(int in[2], int out[2]) {
  dup2(in[0], 0);
  close(in[0]);
  close(in[1]);

  dup2(out[1], 1);
  close(out[1]);
  close(out[0]);

  execlp("dc", "dc", "-", NULL);
}

extern void be_bc(int in[2], int out[2]) {
  close(in[0]);
  close(out[1]);

  FILE *fpin = fdopen(in[1], "w");
  FILE *fpout = fdopen(out[0], "r");

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
