#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#include "dgram.h"

extern void say_who_called(struct sockaddr_in saddr);
extern void reply_to_sender(int sock_fd, char *msg, struct sockaddr_in *saddrp, socklen_t len);

int main(int argc, char *argv[]) {
  int portnum = atoi(argv[1]);
  if (argc != 2 || portnum <= 0) {
    fprintf(stderr, "usage: dgrevc port\n");
    exit(EXIT_FAILURE);
  }

  int sock_id = make_dgram_server_socket(portnum);

  char buf[BUFSIZ];
  struct sockaddr_in saddr;
  socklen_t saddrlen = sizeof(saddr);
  int msglen = 0;
  while ((msglen = recvfrom(sock_id, buf, BUFSIZ, 0, (struct sockaddr *)&saddr, &saddrlen)) > 0) {
    buf[msglen] = '\0';
    printf("dgrecv: got a message: %s\n", buf);
    say_who_called(saddr);
  }
  return EXIT_SUCCESS;
}

extern void say_who_called(struct sockaddr_in saddr) {
  char host[BUFSIZ];
  int port;
  get_internet_address(host, BUFSIZ, &port, &saddr);
  printf("from: %s:%d\n", host, port);
}

extern void reply_to_sender(int sock_fd, char *msg, struct sockaddr_in *saddrp, socklen_t len) {
  char reply[BUFSIZ + BUFSIZ];
  sprintf(reply, "Thanks for your %d char message\n", (socklen_t )strlen(msg));
  sendto(sock_fd, reply, strlen(reply), 0, (const struct sockaddr *)saddrp, len);
}
