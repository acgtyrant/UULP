#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORTNUM 15000

int main(int argc, char *argv[]) {
  if (argc != 3) exit(EXIT_FAILURE);
  
  int sock_id = socket(AF_INET, SOCK_STREAM, 0);

  struct hostent* hp = gethostbyname(argv[1]);
  struct sockaddr_in saddr;
  memset((void *)&saddr, 0, sizeof(saddr));
  memcpy((void *)&saddr.sin_addr, (const void *)hp->h_addr_list[0], sizeof(hp->h_length));
  saddr.sin_port = PORTNUM;
  saddr.sin_family = AF_INET;

  connect(sock_id, (struct sockaddr *)&saddr, sizeof(saddr));

  write(sock_id, argv[2], strlen(argv[2]));
  write(sock_id, "\n", 1);

  int buflen;
  char buf[BUFSIZ];
  while ((buflen = read(sock_id, buf, BUFSIZ)) != 0)
    write(1, buf, buflen);

  return EXIT_SUCCESS;
}
