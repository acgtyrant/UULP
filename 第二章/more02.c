// more02.c - version 0.2 of more
// read and print 24 lines then pause for a few special commands
// feature of version 0.2: reads from /dev/tty for commands

#include <stdio.h>
#include <stdlib.h>

#define PAGELEN 24
#define LINELEN 512

extern void do_more(FILE *fp);
extern int see_more(void);


int main(int argc, char *argv[]) {
  FILE *fp;
  FILE *fp_tty = fopen("/dev/tty", "w+");

  if (fp_tty == NULL) {
    printf("Can not open or write /dev/tty!\n");
    exit(EXIT_FAILURE);
  }
  if (argc == 1)
    do_more(stdin);
  else 
    while (--argc) {
      if ((fp = fopen(*++argv, "r")) != NULL) {
        do_more(fp);
        fclose(fp);
      }
      else {
        printf("%s is an invalid file!\n", *argv);
        exit(EXIT_FAILURE);
      }
    }
  return EXIT_SUCCESS;
}

// 先打印行数：当行数达到 PAGELEN 时调用 see_more 函数处理命令；当输出到 stdout
// 时发现 EOF, 自然终止。
extern void do_more(FILE *fp) {
  char line[LINELEN];
  int num_of_lines = 0;
  int reply;

  while (fgets(line, LINELEN, fp)) {
    if (num_of_lines == PAGELEN) {
      reply = see_more();
      if (reply == 0) {
        exit(EXIT_SUCCESS);
      }
      num_of_lines -= reply;

    }
    if (fputs(line, stdout) == EOF) {
      printf("No more lines.\n");
      exit(EXIT_SUCCESS);
    }
    ++num_of_lines;
  }
}

// 返回所要继续显示的行数。
extern int see_more(void) {
  int c;

  printf("\033[7m more? \33[m");

  while ((c = getc(fp_tty)) != EOF) {
    switch(c) {
      case 'q':
        return 0;
      case '\n':
        return 1;
      case ' ':
        return PAGELEN;
    }
  }
  return 0;
}
