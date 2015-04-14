// 为了不让 YCM 抱怨 FILE 类型未知，加上这句好了：
#include <stdio.h>

#define YES 1
#define NO  0

// splitline.c
extern char *next_cmd(char *prompt, FILE *fp);
extern char **splitline(char *cmdline);
extern void freelist(char **arglist);

// execute.c
extern int execute(char **);

// smsh1.c
extern void fatal(char *s1, char *s2, int n);
