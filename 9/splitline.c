// emalloc(), erealloc(), newstr() 这些函数在别的源代码文件都用不到，
// 所以改成静态函数。


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

#define is_delim(x) ((x) == ' ' || (x) == '\t')

static char *newstr(char *s, int len);
static void *emalloc(int size);
static void *erealloc(void *rv, int size);

extern char *next_cmd(char *prompt, FILE *fp) {
  char *buf;
  int bufspace = 0;
  int pos = 0;
  int c;

  printf("%s", prompt);
  while ((c = getc(fp)) != EOF) {
    if (pos + 1 > bufspace) {
      if (bufspace == 0)
        buf = (char *)emalloc(BUFSIZ);
      else
        buf = (char *)erealloc(buf, bufspace + BUFSIZ);
      bufspace += BUFSIZ;
    }
    if (c == '\n')
      break;
    buf[pos++] = c;
  }
  if (c == EOF && pos == 0)
    return NULL;
  buf[pos] = '\0';
  return buf;
}

extern char **splitline(char *line) {
  char **arglist;
  int spots = 0;
  int bufspace = 0;
  int argnum = 0;
  char *cp = line;
  char *start;
  int len;

  if (line == NULL)
    return NULL;

  arglist = (char **)emalloc(BUFSIZ);
  bufspace = BUFSIZ;
  spots = BUFSIZ/sizeof(char *);

  while (*cp != '\0') {
    while (is_delim(*cp))
      cp++;
    if (*cp == '\0')
      break;
    if (argnum + 1 >= spots) {
      arglist = (char **)erealloc(arglist, bufspace + BUFSIZ);
      bufspace += BUFSIZ;
      spots += BUFSIZ/sizeof(char *);
    }

    start = cp;
    len = 1;
    while (*++cp != '\0' && !(is_delim(*cp)))
      len++;
    arglist[argnum++] = newstr(start, len);
  }
  arglist[argnum] = NULL;
  return arglist;
}

extern void freelist(char **arglist) {
  char **cp = arglist;
  while (*cp)
    free(*cp++);
  free(arglist);
}

static char *newstr(char *s, int len) {
  char *rv = (char *)emalloc(len + 1);
  rv[len] = '\0';
  strncpy(rv, s, len);
  return rv;
}

static void *emalloc(int size) {
  void *rv;
  if ((rv = malloc(size)) == NULL)
    fatal("out of memory", "", 1);
  return rv;
}

static void *erealloc(void *rv, int size) {
  void *new_rv;
  if ((new_rv = realloc(rv, size)) == NULL)
    fatal("realloc failed", "", 1);
  return new_rv;
}
