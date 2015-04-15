#include "smsh.h"

extern int process(char **arglist) {
  int rv = 0;

  if (arglist == NULL)
    rv = 0;
  else if (is_control_command(arglist[0]))
    rv = do_control_command(arglist);
  else if (ok_to_execute())
    rv = execute(arglist);

  return rv;
}
