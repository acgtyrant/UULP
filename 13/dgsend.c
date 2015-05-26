#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "dgram.h"

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "usage: dgsend host port 'message'\n");
    exit(EXIT_FAILURE);
  }
  int sock_id = make_dgram_client_socket();

  struct sockaddr_in saddr;
  make_internet_address(argv[1], atoi(argv[2]), &saddr);
  sendto(sock_id, argv[2], sizeof(argv[3]), 0, (const struct sockaddr *)&saddr, sizeof(saddr));
  return EXIT_SUCCESS;
}
