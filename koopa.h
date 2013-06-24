#ifndef KOOPA_H
#define KOOPA_H

#include "process.h"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> /* wait() */
#include <sstream>
#include <vector>
#include <termios.h>

namespace koopa {
  /* Set koopa as current shell.
   * Take control of terminal.
   * Turn off job control signals.
   * Make a process group.
   */
  void init_shell();

  /* Divide args into job(s).
   * Execute args.
   */
  void launch_args();

  /* Execute process. */
  void launch_process(process);
}

#endif
