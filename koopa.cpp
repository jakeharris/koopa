#include "koopa.h" 

#include <sstream>

using namespace koopa; 

pid_t shell_pgid; 
struct termios shell_tmodes; 
int shell_terminal; 
int shell_is_interactive;
 
void koopa::init_shell() { 

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

int main() {
  init_shell();
  
  launch_args();

  return 0;
}

void koopa::launch_args() {
  const std::string KOOPA_SHELL = "koopa$ ";
  const std::string EXIT_CMD = "exit\0";
  const int MAX_WORDS = 100000;
  char **argv;
  std::string curr;
  std::string currLine;
  std::string cmd = "";
  struct process p;

  while(cmd != EXIT_CMD){
    std::cout << KOOPA_SHELL;
    std::getline(std::cin, currLine);
    std::istringstream line(currLine);
    int x = 0;
    while(std::getline(line, curr, ' ')){
      x++;
    }
    argv = new char*[x];
    x = 0; line.str(""); line.clear(); line.str(currLine); cmd = "";
    while(std::getline(line, curr, ' ')){
      if(cmd == "") cmd = curr;
      if(cmd == EXIT_CMD) continue;
      argv[x++] = (char*)curr.c_str();
    }
    pid_t pid = fork();
    if(pid == 0){
      p.argv = argv;
      launch_process(p);
    }
    else if (pid < 0 ) {
      std::cout << "Couldn't properly fork." << std::endl;
      exit(1);
    }
    else {
      p.pid = pid;
    }
  }
}

void koopa::launch_process(process p) {
  execvp(p.argv[0], p.argv);
  exit(0);
}
