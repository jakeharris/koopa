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
  const std::string EXIT_CMD = "exit";
  const int MAX_WORDS = 100000;
  char **argv;
  std::string curr;
  std::string cmd = "";

  while(cmd != EXIT_CMD){
    std::cout << KOOPA_SHELL;
    std::getline(std::cin, curr);
    std::istringstream line(curr);
    int x = 0;
    while(std::getline(line, curr, ' ')){
      if(cmd == "") cmd = curr;
      argv[x++] = (char*)curr.c_str();
    }
    for(int y = 0; y < x; y++) {
      std::cout << "argv[" << y << "]: " << argv[y] << std::endl;
    }
    std::cout << argv[0] << " == exit: " << (argv[0] == "exit") << std::endl;
  }




  /*std::string curr = "";
  std::string argv = "";
  std::string cmd_s = "";
  const char* cmd = "";*/

  /*while(cmd_s.substr(0, 4) != "exit"){
    argv = "";
    cmd = "";
    std::cout << "koopa$ ";
    std::getline(std::cin, curr);
    std::istringstream line(curr);
    while(std::getline(line, curr, ' ')){
      if(cmd == "" || cmd == NULL){
        cmd = (char*)curr.c_str();
        cmd_s = curr;
      }
      argv += curr + " ";
    }
    for(int x = 0; x < argv.size(); x++){
      std::cout << argv[x] << std::endl;
    }*/
    /* execv(cmd, NULL); */
  
}

void launch_job(job j) {

}

void launch_process(process p) {

}
