#include "koopa.h"
#include "job.h"

#include <unistd.h>

using namespace koopa;

pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;

int main() {
  init_shell();

  char *argv[];
  std::getline (std::cin, argv);

  for(int x = 0; x < argv.size(); x++) {
     std::cout << argv[x] << std::endl;
  } 

  return 0;
}

void init_shell() {
     
  /* See if we are running interactively.  */
  shell_terminal = STDIN_FILENO;
  shell_is_interactive = isatty (shell_terminal);
     
  if (shell_is_interactive) {
    /* Loop until we are in the foreground.  */
    while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
      kill (- shell_pgid, SIGTTIN);
     
    /* Ignore interactive and job-control signals.  */
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    signal (SIGCHLD, SIG_IGN);
     
    /* Put ourselves in our own process group.  */
    shell_pgid = getpid ();
    if (setpgid (shell_pgid, shell_pgid) < 0) {
       perror ("Couldn't put the shell in its own process group");
       exit (1);
    }
     
    /* Grab control of the terminal.  */
    tcsetpgrp (shell_terminal, shell_pgid);

    /* Save default terminal attributes for shell.  */
    tcgetattr (shell_terminal, &shell_tmodes);
  }
}

void launch_args(string args) {

}

void launch_job(job job) {

}

void launch_process(process process) {

}
