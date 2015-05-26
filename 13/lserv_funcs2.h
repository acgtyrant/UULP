extern int setup(void);
extern void narrate(char *msg1, char *msg2, struct sockaddr_in *);
extern void handle_request(char *request, struct sockaddr_in *clientp, socklen_t addlen);
extern void ticket_reclaim(int);
