// rotate.c: map a->b, b->c, .. z->a
//  purpose: useful for showing tty modes
// 我一直以为「退格」就是直接删一个字符，不过原来所谓的「退格字符」只是退一个格，
// 如果要真删除一个字符，还得重新插入「空格字符」，并再加「退格字符」，
// 即退一个格才行。
// 仿照 line_len 变量构造 word_len 数组。
// 每次遇到新空格时，就 Push 新的 word_len 进去，其值为 1.
// 每次执行 word_erase 时，Pop 出 word_len 并按其值退格即可。

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <termios.h>

extern void tty_mode(int how);
extern void set_cr_noecho_mode(void);
extern void signal_handler(int signum);

int main(void) {
  tty_mode(0); // save tty mode
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGTERM, signal_handler);
  set_cr_noecho_mode();
  int c;
  int line_len = 0;

  struct termios term;
  tcgetattr(0, &term);
  char erase_c = term.c_cc[VERASE];
  char line_erase_c = term.c_cc[VKILL];
  while ((c = getchar()) != EOF) {
    if (c == ' ') {
      putchar(' ');
      line_len++;
    }

    if (c == '\n') {
      puts("\r");
      line_len = 0;
    }

    if (c == erase_c) {
      fputs("\b \b", stdout);
      line_len--;
    }

    if (c == line_erase_c) {
      while (line_len != 0) {
        fputs("\b \b", stdout);
        line_len--;
      }
    }

    if (c == 'Q') {
      tty_mode(1);
      exit(EXIT_SUCCESS);
    }

    if (islower(c)) {
      if (c == 'z')
        c = 'a';
      else
        c++;
      putchar(c);
      line_len++;
    }
  }
  tty_mode(1);
  return EXIT_SUCCESS;
}

extern void tty_mode(int how) {
  static struct termios original_mode;
  if (how == 0) {
    tcgetattr(0, &original_mode);
  }
  else {
    tcsetattr(0, TCSANOW, &original_mode);
  }
}

extern void set_cr_noecho_mode(void) {
  struct termios ttystate;
  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ICANON; // no buffering
  ttystate.c_lflag &= ~ECHO; // no echo either
  ttystate.c_cc[VMIN] = 1; // get one char at a time
  tcsetattr(0, TCSANOW, &ttystate);
}

extern void signal_handler(int signum) {
  tty_mode(1);
  exit(2);
}
