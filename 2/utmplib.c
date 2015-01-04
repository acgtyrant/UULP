#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>

#define NERCS 16
#define NULLUT ((struct utmp *) NULL)
#define UTSIZE (sizeof(struct utmp))

extern int utmp_open(char *filename);
extern struct utmp *utmp_next(void);
extern int utmp_reload(void);
extern void utmp_close(void);

//此源文件用全局变量，我觉得这并不是好习惯。
static char utmpbuf[NERCS * UTSIZE];
static int num_recs;
static int cur_rec;
static int utmp_fd = -1;

extern int utmp_open(char *filename) {
  utmp_fd = open(filename, O_RDONLY);
  cur_rec = num_recs = 0;
  return utmp_fd;
}

extern struct utmp *utmp_next(void) {
  if (utmp_fd == -1)
    return NULLUT;
  //如果缓冲记录读到头了，才会接着执行下一条判断句，即从文件重新加载新记录到缓冲去；
  //如果文件记录也读完了，第二个判断句就返回 0, 从而判断条件生效，最终返回空指针。
  if (cur_rec == num_recs && utmp_reload() == 0)
    return NULLUT;

  struct utmp *recp = (struct utmp *) utmpbuf[cur_rec * UTSIZE];
  ++cur_rec;
  return recp;
}

extern int utmp_reload(void) {
  int amt_read;
  amt_read = read(utmp_fd, utmpbuf, NERCS * UTSIZE);
  num_recs = amt_read / UTSIZE;
  cur_rec = 0;
  return num_recs;
}

//与 utmp_open() 不同，没有接受文件名，而是直接操作全局变量，太乱来。
extern void utmp_close(void) {
  if (utmp_fd != -1)
    close(utmp_fd);
}
