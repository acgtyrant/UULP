#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define HOSTLEN 256
#define oops(msg) {perror(msg); exit(EXIT_FAILURE);}
#define PORTNUM 12345

static bool is_addr_vald(struct in_addr addr);

int main(void) {
  int sock_id = socket(PF_INET, SOCK_STREAM, 0);
  if (sock_id == -1)
    oops("socket");

  char hostname[HOSTLEN];
  gethostname(hostname, HOSTLEN);
  // char hostname[] = "127.0.0.1";
  struct hostent *hp = gethostbyname(hostname);

  struct sockaddr_in saddr;
  memset((void *)&saddr, 0, sizeof(saddr));
  memcpy((void *)&saddr.sin_addr, (const void *)hp->h_addr_list[0], sizeof(hp->h_length));
  // h_addr is defined as h_addr_list[0] for backward compatibility actually
  saddr.sin_port = htons(PORTNUM);
  saddr.sin_family = AF_INET;

  // 避免 bind: Address already in use 错误
  // 参见 http://blog.csdn.net/piaojun_pj/article/details/6098438
  int on = 1;
  if (setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    oops("setsockopt");

  if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
    // 如果遇到 bind: Cannot assign requested address 错误
    // 就要在 /etc/hosts 里把 hostname 手动加进去
    oops("bind");

  if (listen(sock_id, 1) != 0)
    oops("listen");

  int sock_fd;
  FILE *sock_fp;
  time_t thetime;
  socklen_t saddr_len = sizeof(saddr);
  while (true) {
    sock_fd = accept(sock_id, (struct sockaddr *)&saddr, &saddr_len);
    if (sock_fd == -1)
      oops("accept");
    socklen_t addr_len = sizeof(saddr.sin_addr);
    hp = gethostbyaddr(&saddr.sin_addr, addr_len, AF_INET);
    if (is_addr_vald(saddr.sin_addr) == false) {
      printf("Invalid address: %s \n", inet_ntoa(saddr.sin_addr));
      close(sock_fd);
      continue;
    }
    printf("Got ad call from %s (%s)\n", inet_ntoa(saddr.sin_addr), hp->h_name);
    fflush(stdout);

    sock_fp = fdopen(sock_fd, "w");
    if (sock_fp == NULL)
      oops("fdopen");

    thetime = time(NULL);

    fprintf(sock_fp, "The time here is %s.\n", ctime(&thetime));
    fclose(sock_fp);
  }
  
  close(sock_id);
  return EXIT_SUCCESS;
}

static bool is_addr_vald(struct in_addr addr) {
  char addr_a[16]; // 注意地址长度不一定是十六位！
  strncpy(addr_a, inet_ntoa(addr), strlen(inet_ntoa(addr)));
  FILE *whitelist = fopen("whitelist", "r");
  char buf[BUFSIZ];
  while (fgets(buf, BUFSIZ, whitelist) != NULL) {
    if (buf[strlen(buf) - 1] == '\n')
      buf[strlen(buf) - 1] = '\0';
    if (strncmp(buf, addr_a, strlen(buf)) == 0)
      return true;
  }

  return false;
}
