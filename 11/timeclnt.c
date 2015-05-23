#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct sockaddr_in saddr;

  int sock_id = socket(AF_INET, SOCK_STREAM, 0);

  struct hostent *hp = gethostbyname(argv[1]);
  memset((void *)&saddr, 0, sizeof(saddr));
  memcpy((void *)&saddr.sin_addr, (const void *)hp->h_addr_list[0], sizeof(hp->h_length));
  saddr.sin_port = htons(atoi(argv[2]));
  saddr.sin_family = AF_INET;

  connect(sock_id, (struct sockaddr *)&saddr, sizeof(saddr));

  char message[BUFSIZ];
  int messlen = read(sock_id, message, BUFSIZ);
  write(1, message, messlen);
  close(sock_id);
  return EXIT_SUCCESS;
}
