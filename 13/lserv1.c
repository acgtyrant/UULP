#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdbool.h>

#include "dgram.h"
#include "lserv_funcs1.h"

#define MSGLEN 128

int main(void) {
  struct sockaddr_in clnt_addr;
  socklen_t addrlen = sizeof(clnt_addr);
  int ret;
  char buf[MSGLEN];

  int sock_fd = setup();
  while (true) {
    ret = recvfrom(sock_fd, buf, MSGLEN, 0, (struct sockaddr *)&clnt_addr, &addrlen);
    if (ret != -1) {
      buf[ret] = '\0';
      narrate("GOT:", buf, &clnt_addr);
      handle_request(buf, &clnt_addr, addrlen);
    }
    else {
      if (errno != EINTR)
        perror("recvfrom");
    }
  }

  return EXIT_SUCCESS;
}
