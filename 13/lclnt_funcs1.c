#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include "lclnt_funcs1.h"
#include "dgram.h"

static int pid = -1;
static int sock_fd = -1;
static struct sockaddr_in serv_addr;
static socklen_t serv_alen;
static char ticket_buf[128];
static int have_ticket = 0;

#define MSGLEN 128
#define SERVER_PORTNUM 2020
#define HOSTLEN 512
#define oops(p) {perror(p); exit(1);}

static char *do_transaction(char *);
static void narrate(char *msg1, char *msg2);
static void syserr(char *msg1);

extern void setup(void) {
  char hostname[HOSTLEN];

  pid = getpid();
  sock_fd = make_dgram_client_socket();
  gethostname(hostname, HOSTLEN);
  make_internet_address(hostname, SERVER_PORTNUM, &serv_addr);
  serv_alen = sizeof(serv_addr);
}

extern int get_ticket(void) {
  if (have_ticket)
    return 0;
  char msg[MSGLEN];
  char *respone;
  sprintf(msg, "HELO %d", pid);
  if ((respone = do_transaction(msg)) == NULL)
    return -1;

  if (strncmp(respone, "TICK", 4) == 0) {
    strcpy(ticket_buf, respone + 5);
    have_ticket = 1;
    narrate("got ticket", ticket_buf);
    return 0;
  }

  if (strncmp(respone, "FAIL", 4) == 0) {
    narrate("Could not get ticket:", respone);
  }
  else {
    narrate("Unknown message:", respone);
  }
  
  return -1;
}

extern int release_ticket(void) {
  char msg[MSGLEN];
  char *respone;

  sprintf(msg, "GBYE %s", ticket_buf);
  if ((respone = do_transaction(msg)) == NULL)
    return -1;

  if (strncmp(respone, "THNX", 4) == 0) {
    narrate("released ticket OK", "");
    return 0;
  }

  if (strncmp(respone, "FAIL", 4) == 0) {
    narrate("release_ticket FAIL", respone + 5);
  }
  else {
    narrate("Unknown message:", respone);
  }

  return -1;
}

static char *do_transaction(char *msg) {
  int ret;
  ret = sendto(sock_fd, msg, strlen(msg), 0, (struct sockaddr *)&serv_addr, serv_alen);
  if (ret == -1) {
    syserr("sendto");
    return NULL;
  }

  static char buf[MSGLEN]; // 突破栈限制
  ret = recvfrom(sock_fd, buf, MSGLEN, 0, NULL, NULL);
  if (ret == -1) {
    syserr("sendto");
    return NULL;
  }

  return buf;
}

extern void shut_down(void) {
  close(sock_fd);
}

static void narrate(char *msg1, char *msg2) {
  fprintf(stderr, "CLIENT [%d]: %s %s\n", pid, msg1, msg2);
}

static void syserr(char *msg1) {
  char buf[BUFSIZ];
  sprintf(buf, "CLIENT [%d]: %s", pid, msg1);
  perror(buf);
}
