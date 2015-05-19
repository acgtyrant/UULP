#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

#define PORTNUM 15000

extern void sanitize(char *);

int main(void) {
  int sock_id = socket(PF_INET, SOCK_STREAM, 0);

  char hostname[BUFSIZ];
  gethostname(hostname, BUFSIZ);
  struct hostent *hp = gethostbyname(hostname);
  
  struct sockaddr_in saddr;
  bzero((void *)&saddr, sizeof(saddr));
  bcopy((void *)hp->h_addr_list[0], (void *)&saddr.sin_addr, hp->h_length);
  saddr.sin_port = PORTNUM;
  saddr.sin_family = AF_INET;

  bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr));
  listen(sock_id, 1);

  int sock_fd;
  FILE *sock_fpi, *sock_fpo, *pipe_fp;
  char dirname[BUFSIZ];
  char command[BUFSIZ];
  int c;
  while (true) {
    sock_fd = accept(sock_id, NULL, NULL);
    sock_fpi = fdopen(sock_fd, "r");
    fgets(dirname, BUFSIZ, sock_fpi);
    sanitize(dirname);
    sock_fpo = fdopen(sock_fd, "w");
    sprintf(command, "ls %s", dirname);
    pipe_fp = popen(command, "r");
    while ((c = getc(pipe_fp)) != EOF)
      putc(c, sock_fpo);
    pclose(pipe_fp);
    fclose(sock_fpi);
    fclose(sock_fpo);
  }

  close(sock_id);
  close(sock_fd);
  return EXIT_SUCCESS;
}

extern void sanitize(char *str) {
  char *src, *dest;

  for (src = dest = str; *src; src++)
    if (*src == '/' || isalnum(*src))
      *dest++ = *src;

  *dest = '\0';
}
