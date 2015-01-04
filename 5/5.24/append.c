// (1) 当链接文件已存在，则 link() 调用失败，代表已上锁
// (2) 参见该源代码
// (3) 参见 (!)
// (4) 查 utils-linux 源代码，证实其锁为链接。


#include <stdio.h>
#include <stdlib.h> // EXIT_SUCCESS
#include <errno.h>
#include <unistd.h> // link()
#include <sys/stat.h> // open()
#include <fcntl.h> // open()

#define MAXDELAT 500

extern int lock_txt(void);
extern void unlock_txt(void);

int main(void) {
  int respone = 0;
  int delay = MAXDELAT;
  char sentence[] = "I am a sentence!";
  size_t sentence_len = sizeof(sentence);

  while (delay != 0 && (respone = lock_txt()) == 1) {
    sleep(1);
    --delay;
  }
  if (respone == 0) {
    int txt_fd = open("txt", O_APPEND);
    write(txt_fd, sentence, sentence_len);
    unlock_txt();
    close(txt_fd);
  }
  else {
    printf("Failed to append!\n");
    
  }

  return EXIT_SUCCESS;
}

extern int lock_txt(void) {
  int respone = 0;
  if (link("txt", "txt.LCK") == -1)
    respone = (errno == EEXIST ? 1 : 2);
  return respone;
}
