#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST
#define NULLUT ((struct utmp *) NULL)

extern void show_info(struct utmp *utmp_record);
//不用头文件，直接声明全在 utmplib.c 的函数。
extern int utmp_open(char *filename);
extern struct utmp *utmp_next(void);
extern int utmp_reload(void);
extern void utmp_close(void);

int main(int argc, char *argv[]) {
  struct utmp *utbufp;
  int utmp_fd = utmp_open(UTMP_FILE);

  if (utmp_fd == -1) {
    perror(UTMP_FILE);
    exit(EXIT_FAILURE);
  }

  while ((utbufp = utmp_next()) != NULLUT)
    show_info(utbufp);

  utmp_close();
  return EXIT_SUCCESS;
}

extern void show_info(struct utmp *utmp_record) {
  if (utmp_record->ut_type != USER_PROCESS) return;

  printf("%-8.8s", utmp_record->ut_user);
  printf(" ");
  printf("%-8.8s", utmp_record->ut_line);
  printf(" ");
  printf("%12.12s", ctime(&utmp_record->ut_time) + 4);
  printf(" ");
#ifdef SHOWHOST
  printf("(%s)", utmp_record->ut_host);
#endif
  printf("\n");
}
