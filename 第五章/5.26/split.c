// 意外地学到了不少 time.h 的内容。`man 2 time`, `man 2 times`, `man 7 time`,
// `man ctime`, `man time.h`, `man 3 clock` 都值得一读。
//
// 本来想调用 clock() 统计执行时间的，不过还不如直接用 Shell 命令 `time`
// 来得快，又能分成 user time 和 system time.
//
// 首先用 `dd if=/dev/zero of=2M.text bs=20M count=1` 在当前目录生成即可。
//
// Usage: ./a.out y|n partsize, y|n 决定是否用 O_SYNC, partsize 则决定分片大小。
//
// 试验结果如下：
//
// y, partsize = 16: ./a.out  0.04s user 1.41s system 94% cpu 1.533 total
// n, partsize = 16: ./a.out  0.05s user 0.35s system 99% cpu 0.404 total
//
// y, partsize = 1000: ./a.out y 1000  0.00s user 0.02s system 96% cpu 0.024 total
// n, partsize = 1000: ./a.out n 1000  0.00s user 0.02s system 31% cpu 0.064 total

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // open()
#include <fcntl.h> // open()
#include <unistd.h> // close(), read()
#include <string.h> // strcmp()

#define FILENAMESIZE 10

int main(int argc, char *argv[]) {
  int oflag = O_CREAT;
  if (strcmp(argv[1], "y") == 0)
    oflag |= O_SYNC;
  int partsize = atoi(argv[2]);

  int src_fd = open("2M.text", oflag);
  int dest_fd;
  int index = 1;
  char dst_name[FILENAMESIZE];
  char buf[partsize];
  int respone;
  mkdir("tmp", 0700);
  chdir("tmp");
  while ((respone = read(src_fd, buf, partsize)) > 0) {
    sprintf(dst_name, "%d", index);
    dest_fd = open(dst_name, O_CREAT | O_WRONLY);
    write(dest_fd, buf, respone);
    close (dest_fd);
    ++index;
  }
  if (respone == -1) {
    perror("2M.text");
    exit(EXIT_FAILURE);
  }
  close(src_fd);
  return EXIT_SUCCESS;
}
