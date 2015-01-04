#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s OWNER FILE...\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  uid_t dst_uid;
  struct passwd *passwd_buf;
  while ((passwd_buf = getpwent()) != NULL) {
    if (passwd_buf->pw_uid == atol(argv[1]) || strcmp(passwd_buf->pw_name, (char *)argv[0])) {
      dst_uid = passwd_buf->pw_uid;
      break;
    }
  }
  if (passwd_buf == NULL) {
    fprintf(stderr, "No such uid: %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  for (int i = 2; i < argc; ++i) {
    if (chown(argv[i], dst_uid, -1) == -1) {
      fprintf(stderr, "Failed to chown\n");
      perror(argv[i]);
      exit(EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}
