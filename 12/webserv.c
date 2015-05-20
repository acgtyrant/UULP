#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>

extern int make_server_socket(int port);
extern int make_server_socket_q(int port, int backlog);

extern void process_rq(char *request, int sock_fd);
extern void header(FILE *sock_fp, char *content_type);
extern void cannot_do(int sock_fd);
extern int not_exist(char *f);
extern void do_404(char *item, int sock_fd);
extern int is_dir(char *dir);
extern void do_ls(char *arg, int sock_fd);
extern char *file_type(char *f);
extern int ends_in_cgi(char *arg);
extern void do_exec(char *comm, int sock_fd);
extern void do_cat(char *f, int sock_fd);

int main(int argc, char *argv[]) {
  if (argc != 2) { fprintf(stderr, "usage: ./webserv portnum\n");
    exit(EXIT_FAILURE);
  }

  int sock_id = make_server_socket(atoi(argv[1]));

  int sock_fd;
  char request[BUFSIZ];
  FILE *sock_fp;
  while (true) { // 循环中自然可以多次接收命令
    sock_fd = accept(sock_id, NULL, NULL);
    sock_fp = fdopen(sock_fd, "r");
    fgets(request, BUFSIZ, sock_fp);
    printf("got a call: request = %s\n", request);
    // sock_fp 和 sock_fd 的位置就是同一个，所以必须处理掉 sock_fp 的输出
    char buf[BUFSIZ];
    while (fgets(buf, BUFSIZ, sock_fp) != NULL && strcmp(buf, "\r\n") != 0)
      ;

    process_rq(request, sock_fd);
    fclose(sock_fp);
  }
  return EXIT_SUCCESS;
}

extern void process_rq(char *request, int sock_fd) {
  char cmd[BUFSIZ], arg[BUFSIZ];
  strcpy(arg, "./");
  sscanf(request, "%s %s", cmd, arg + 2);

  if (fork() != 0)
    return ;

  if (strcmp(cmd, "GET") != 0) {
    cannot_do(sock_fd);
  } else if (not_exist(arg)) {
    do_404(arg, sock_fd);
  } else if (is_dir(arg)) {
    do_ls(arg, sock_fd);
  } else if (ends_in_cgi(arg)) {
    do_exec(arg, sock_fd);
  } else {
    do_cat(arg, sock_fd);
  }
}

extern void header(FILE *sock_fp, char *content_type) {
  fprintf(sock_fp, "HTTP/1.0 200 OK\r\n");
  if (content_type)
    fprintf(sock_fp, "Content-type: %s\r\n", content_type);
}

extern void cannot_do(int sock_fd) {
  FILE *sock_fp = fdopen(sock_fd, "w");
  fprintf(sock_fp, "HTTP/1.0 501 Not Implemented\r\n"
                   "Content-type: text/plain\r\n"
                   "\r\n"
                   "That command is not yet implementd\r\n");
  fclose(sock_fp);
}

extern int not_exist(char *f) {
  struct stat info;
  return (stat(f, &info) == -1);
}

extern void do_404(char *item, int sock_fd) {
  FILE *sock_fp = fdopen(sock_fd, "w");
  fprintf(sock_fp, "HTTP/1.0 404 Not Found\r\n"
                   "Content-type: text/plain\r\n"
                   "\r\n"
                   "The item you requested: %s\r\n"
                   "is not found\r\n", item);
  fclose(sock_fp);
}

extern int is_dir(char *dir) {
  struct stat info;
  return (stat(dir, &info) != -1 && S_ISDIR(info.st_mode));
}

extern void do_ls(char *dir, int sock_fd) {
  FILE *sock_fp = fdopen(sock_fd, "w");
  header(sock_fp, "text/plain");
  fprintf(sock_fp, "\r\n");
  fflush(sock_fp);

  dup2(sock_fd, 1);
  dup2(sock_fd, 2);
  close(sock_fd);

  execlp("/bin/ls", "ls", dir, NULL);
  perror(dir);
  exit(EXIT_FAILURE);
}

extern char *file_type(char *f) {
  char *cp;
  if ((cp = strrchr(f, '.')) != NULL)
    return cp + 1;
  return "";
}

extern int ends_in_cgi(char *f) {
  return (strcmp(file_type(f), "cgi") == 0);
}

extern void do_exec(char *comm, int sock_fd) {
  FILE *sock_fp = fdopen(sock_fd, "w");
  header(sock_fp, NULL);
  fflush(sock_fp);

  dup2(sock_fd, 1);
  dup2(sock_fd, 2);
  close(sock_fd);
  execl(comm, comm, NULL);
  perror(comm);
  // 这里没有 exit(), 服务器可以继续接收命令
}

extern void do_cat(char *f, int sock_fd) {
  char *extension = file_type(f);
  char *content = "text/plain";

  if (strcmp(extension, "html") == 0)
    content = "text/html";
  else if (strcmp(extension, "gif") == 0)
    content = "image/gif";
  else if (strcmp(extension, "jpg") == 0)
    content = "image/jpg";
  else if (strcmp(extension, "jpeg") == 0)
    content = "image/jpeg";

  FILE *sock_fp = fdopen(sock_fd, "w");
  FILE *file_fp = fopen(f, "r");
  int c;
  if (sock_fp != NULL && file_fp != NULL) {
    header(sock_fp, NULL);
    fprintf(sock_fp, "\r\n");
    while ((c = getc(file_fp)) != EOF)
      putc(c, sock_fp);
    fclose(sock_fp);
    fclose(file_fp);
  }
  exit(EXIT_SUCCESS);
}
