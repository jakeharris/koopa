#include "koopa.h"

using namespace koopa; 

pid_t shell_pgid; 
struct termios shell_tmodes; 
int shell_terminal; 
int shell_is_interactive;
 
void koopa::init_shell() { 
  /* See if we are running interactively. */ 
  shell_terminal = STDIN_FILENO; 
  shell_is_interactive = isatty (shell_terminal);
     
  if (shell_is_interactive) {
  /* Loop until we are in the foreground. */  
    while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
      kill (- shell_pgid, SIGTTIN);
     
    /* Ignore interactive and job-control signals. */
    /*
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    signal (SIGCHLD, SIG_IGN);
    */
    
    /* Put ourselves in our own process group.*/  
    shell_pgid = getpid ();
    if (setpgid (shell_pgid, shell_pgid) < 0) {
       perror ("Couldn't put the shell in its own process group");
       exit (1);
    }
    
    /* Grab control of the terminal.  */
    tcsetpgrp (shell_terminal, shell_pgid);

    /* Save default terminal attributes for shell.*/  
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
  const std::string CD_CMD = "cd\0";
  const int MAX_WORDS = 100000;
  std::vector<char*> args;
  char **argv;
  std::string curr;
  std::string currLine;
  std::string cmd = "";
  struct process p;
  char hostname[128];

  gethostname(hostname, sizeof hostname);

  while(cmd != EXIT_CMD) {
    curr = "";
    currLine = "";
    args.clear();
    
    /* Display prompt. */
    std::cout << getlogin() << "@" << hostname << ":" << getcwd(NULL, 0) << " " << KOOPA_SHELL;

    /* Get user input as a string. */
    std::getline(std::cin, currLine);

    /* Convert input string to character array. */
    char* unsplit = new char[currLine.size() + 1];
    std::copy(currLine.begin(), currLine.end(), unsplit);
    unsplit[currLine.size()] = '\0';
    
    /* Split command and arguments in character array. */
    char* split = strtok(unsplit, " ");
    
    /* Push arguments to argument vector. */
    while (split != NULL) {
      args.push_back(split);
      split = strtok(NULL, " ");
    }
    
    /* Convert argument vector to double character pointer. */
    argv = new char*[args.size() + 1];
    for (int i = 0; i < args.size(); i++) {
      argv[i] = args[i];
    }
    
    /* Null-terminate the double character pointer. */
    argv[args.size()] = NULL;
    
    /* Change directory. */
    if(cmd == CD_CMD) {
      if(argv[1] == NULL) chdir("/");
      else chdir(argv[1]);
      continue;
    }
    
    pid_t pid = fork();
    if (pid == 0) {
      p.argv = argv;
      launch_process(p);
    }
    else if (pid < 0) {
      std::cout << "Couldn't properly fork." << std::endl;
      exit(1);
    }
    else {
      int waitpidResult = waitpid(pid, 0, 0);
      if (waitpidResult < 0) {
        perror("Internal error: connot wait for child.");
        std::cout << waitpidResult;
        exit(1);
      }
    }
  }
}

void koopa::launch_process(process p) {
  execvp(p.argv[0], p.argv);
  perror(p.argv[0]);
  exit(1);
}
