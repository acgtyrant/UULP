#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define HOSTLEN 256
#define oops(msg) {perror(msg); exit(EXIT_FAILURE);}
#define PORTNUM 12345

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

  if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
    // 如果遇到 bind: Cannot assign requested address 错误
    // 就要在 /etc/hosts 里把 hostname 手动加进去
    oops("bind");

  if (listen(sock_id, 1) != 0)
    oops("listen");

  int sock_fd;
  FILE *sock_fp;
  time_t thetime;
  while (true) {
    sock_fd = accept(sock_id, NULL, NULL);
    if (sock_fd == -1)
      oops("accept");
    printf("Wow! got a call!\n");

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
