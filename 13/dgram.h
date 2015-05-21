extern int make_dgram_server_socket(int portnum);
extern int make_dgram_client_socket(void);
extern void make_internet_address(char *host, int portnum, struct sockaddr_in *saddrp);
extern void get_internet_address(char *host, int hostlen, int *portp, struct sockaddr_in *saddrp);
