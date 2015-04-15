// 注意 syn_err 会重置 if_state, 也就是说遇到语法错误，前面的条件句就全无效。
// 同时还会忽略 then, fi 之后的命令。

#include <stdio.h>
#include <string.h>
#include "smsh.h"

enum states {NEUTRAL, WANT_THEN, THEN_BLOCK};
enum results {SUCCESS, FAIL};

static int if_state = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;

static int syn_err(char *msg) {
  if_state = NEUTRAL;
  fprintf(stderr, "syntax error: %s\n", msg);
  return -1;
}

extern int ok_to_execute(void) {
  int rv = 1;
  if (if_state == WANT_THEN) {
    syn_err("then expercted");
    rv = 0;
  }
  else if (if_state == THEN_BLOCK && if_result == SUCCESS)
    rv = 1;
  else if (if_state == THEN_BLOCK && if_result == FAIL)
    rv = 0;

  return rv;
}

extern int is_control_command(char *s) {
  return (strcmp(s, "if") == 0 || strcmp(s, "then") == 0 ||
          strcmp(s, "fi") == 0);
}

extern int do_control_command(char **arglist) {
  char *cmd = arglist[0];
  int rv = -1;

  // 教材版在这里可能有若 if 后面无任何命令，则崩溃的 bug.
  if (strcmp(cmd, "if") == 0) {
    if (if_state != NEUTRAL) {
      rv = syn_err("if unexpected");
    } 
    // 解决方案是再加上判断 arglist 有无其它命令的条件句
    else if (arglist[1] == NULL) {
      rv = syn_err("if statement expected");
    }
    else {
      last_stat = process(arglist + 1);
      if_result = (last_stat == 0 ? SUCCESS : FAIL);
      if_state = WANT_THEN;
      rv = 0;
    }
  }

  if (strcmp(cmd, "then") == 0) {
    if (if_state != WANT_THEN) {
      rv = syn_err("then expected");
    }
    else {
      if_state = THEN_BLOCK;
      rv = 0;
    }
  }

  if (strcmp(cmd, "fi") == 0) {
    if (if_state != THEN_BLOCK) {
      rv = syn_err("fi unexpected");
    }
    else {
      if_state = NEUTRAL;
      rv = 0;
    }
  }

  return rv;
}
