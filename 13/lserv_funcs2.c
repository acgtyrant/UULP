#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/errno.h>
#include <string.h>

#include "lserv_funcs1.h"
#include "dgram.h"

#define SERVER_PORTNUM 2020
#define MSGLEN 128
#define TICKET_AVAIL 0
#define MAXUSERS 3
#define oops(x) {perror(x); exit(EXIT_FAILURE);}
#define RECLAIM_INTERVAL 60

static int ticket_array[MAXUSERS];
static int sock_fd = -1;
static int num_tickets_out = 0;

static void free_all_tickets(void);
static char *do_hello();
static char *do_goodbye();
static char *do_validate(char *msg);

extern int setup(void) {
  sock_fd = make_dgram_server_socket(SERVER_PORTNUM);
  if (sock_fd == -1)
    oops("make socket");
  free_all_tickets();
  return sock_fd;
}

extern void narrate(char *msg1, char *msg2, struct sockaddr_in *clientp) {
  fprintf(stderr, "SERVER: %s %s", msg1, msg2);
  if (clientp)
    fprintf(stderr, "(%s: %d)\n", inet_ntoa(clientp->sin_addr), ntohs(clientp->sin_port));
}

extern void handle_request(char *request, struct sockaddr_in *clientp, socklen_t addlen) {
  char *respone;

  if (strncmp(request, "HELO", 4) == 0)
    respone = do_hello(request);
  else if (strncmp(request, "GBYE", 4) == 0)
    respone = do_goodbye(request);
  else if (strncmp(request, "VALD", 4) == 0)
    respone = do_validate(request);
  else
    respone = "FAIL invalid request";

  narrate("SAID", respone, clientp);
  int ret = sendto(sock_fd, respone, strlen(respone), 0, (const struct sockaddr *)clientp, addlen);
  if (ret == -1)
    perror("SERVER sendto failed"); 
}

extern void ticket_reclaim(int a) {
  a += 1; // Unuseful param, just set this line to ignore warning
  char ticket[BUFSIZ];
  for (int i = 0; i < MAXUSERS; ++i) {
    if (ticket_array[i] != TICKET_AVAIL &&
        kill(ticket_array[i], 0) == -1 &&
        errno == ESRCH) {
      sprintf(ticket, "%d.%d", ticket_array[i], i);
      narrate("freeing", ticket, NULL);
      ticket_array[i] = TICKET_AVAIL;
      num_tickets_out--;
    }
  }
  alarm(RECLAIM_INTERVAL);
}

static void free_all_tickets(void) {
  for (int i = 0; i < MAXUSERS; ++i)
    ticket_array[i] = TICKET_AVAIL;
}

static char *do_hello(char *request) {
  static char respone[MSGLEN];

  if (num_tickets_out >= MAXUSERS)
    return "FAIL no tickets availble";

  int x;
  for (x = 0; x < MAXUSERS && ticket_array[x] != TICKET_AVAIL; x++) ;

  if (x == MAXUSERS) {
    narrate("database corrupt", "", NULL);
    return "FAIL databse corrupt";
  }

  ticket_array[x] = atoi(request + 5);
  num_tickets_out++;
  sprintf(respone, "TICK %d.%d", ticket_array[x], x);
  return respone;
}

static char *do_goodbye(char *request) {
  int pid, x;
  if ((sscanf((request + 5), "%d.%d", &pid, &x) != 2 ) ||
      (ticket_array[x] != pid)) {
    narrate("Bogus ticket", request + 5, NULL);
    return "FAIL invalid ticket";
  }

  ticket_array[x] = TICKET_AVAIL;
  num_tickets_out--;
  return "THNX See ya!";
}

static char *do_validate(char *request) {
  int pid, x;
  if (sscanf(request + 5, "%d.%d", &pid, &x) == 2 && ticket_array[x] == pid)
    return "GOOD Valid ticket";

  narrate("Bogus ticket", request + 5, NULL);
  return "FAIL invalid tickert";
}
