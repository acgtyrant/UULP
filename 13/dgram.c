#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h> // struct hostent, gethostbyname
#include <arpa/inet.h>

extern void make_internet_address(char *host, int portnum, struct sockaddr_in *saddrp);
#define HOSTLEN 256

extern int make_dgram_server_socket(int portnum) {
  int sock_id = socket(PF_INET, SOCK_DGRAM, 0);
  char hostname[HOSTLEN];
  gethostname(hostname, HOSTLEN);
  struct sockaddr_in saddr;
  make_internet_address(hostname, portnum, &saddr);
  bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr));

  return sock_id;
}
extern int make_dgram_client_socket(void) {
  return socket(AF_INET, SOCK_DGRAM, 0);
}
extern void make_internet_address(char *host, int portnum, struct sockaddr_in *saddrp) {
  struct hostent *hp = gethostbyname(host);
  memset((void *)saddrp, 0, sizeof(saddrp));
  memcpy((void *)&saddrp->sin_addr, (const void *)hp->h_addr_list[0], hp->h_length);
  saddrp->sin_port = htons(portnum);
  saddrp->sin_family = AF_INET;
}
extern void get_internet_address(char *host, int hostlen, int *portp, struct sockaddr_in *saddrp) {
  strncpy(host, inet_ntoa(saddrp->sin_addr), hostlen);
  *portp = ntohs(saddrp->sin_port);
}
