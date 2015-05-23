#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#define HOSTLEN 256
#define BACKLOG 1

extern int make_server_socket(int port);
extern int make_server_socket_q(int port, int backlog);

extern int make_server_socket(int port) {
  return make_server_socket_q(port, BACKLOG);
}

extern int make_server_socket_q(int port, int backlog) {
  int sock_id = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_id == -1)
    return -1;

  char hostname[HOSTLEN];
  gethostname(hostname, HOSTLEN);
  struct hostent *hp = gethostbyname(hostname);

  struct sockaddr_in saddr;
  memset((void *)&saddr, 0, sizeof(saddr));
  memcpy((void *)&saddr.sin_addr, (const void *)hp->h_addr_list[0], sizeof(hp->h_length));
  saddr.sin_port = htons(port);
  saddr.sin_family = AF_INET;

  if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
    return -1;

  if (listen(sock_id, backlog))
    return -1;

  return sock_id;
}

extern int connect_to_server(char *hostname, int port) {
  int sock_id = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_id == -1)
    return -1;

  struct hostent *hp = gethostbyname(hostname);
  if (hp == NULL)
    return -1;

  struct sockaddr_in saddr;
  memset((void *)&saddr, 0, sizeof(saddr));
  memcpy((void *)&saddr.sin_addr, (const void *)hp->h_addr_list[0], sizeof(hp->h_length));
  saddr.sin_port = port;
  saddr.sin_family = AF_INET;

  if (connect(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
    return -1;

  return sock_id;
}
