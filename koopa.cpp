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
  /* Initialize constants. */
  const std::string KOOPA_SHELL = "koopa$ ";
  const std::string EXIT_CMD = "exit\0";
  const std::string CD_CMD = "cd\0";

  /* Allocate memory for all of our arguments and our process. */
  std::vector<char*> args;
  char **argv;
  std::string curr;
  std::string currLine;
  std::string cmd = "";
  struct process p;
  char hostname[128];

  /* Get *nix hostname (machine name). */
  gethostname(hostname, sizeof hostname);
  
  while(cmd != EXIT_CMD) {
    /* Clear everything out from our last run, if one existed. */
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
    
    /* Prevent the segfault caused by pressing enter without typing a command. */
    if (currLine == "\0") continue;
    
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
    
    /* Set cmd. */
    cmd = argv[0];
    
    /* Case: Change directory. */
    if (cmd == CD_CMD) {
      if (argv[1] == NULL) chdir("/");
      else chdir(argv[1]);
    
    /* Case: Exit. */
    } else if (cmd == EXIT_CMD) {
      exit(1);
      
    /* Case: Other. */
    } else {
      /* Fork a new process. */
      pid_t pid = fork();
      /* If we made a new, blank process successfully, */
      if (pid == 0) {
        /* then attach our input arguments to the process's data and launch that onto the newly forked process. */
        p.argv = argv;
        launch_process(p);
      }
      /* Otherwise, if we failed to make a process, */
      else if (pid < 0) {
        /* print an error message and exit the shell. */
        std::cout << "Couldn't properly fork." << std::endl;
        exit(1);
      }
      /* Otherwise, */
      else {
        /* wait. */
        int waitpidResult = waitpid(pid, 0, 0);
        /* If there is an error while waiting, */
        if (waitpidResult < 0) {
          /* then print an error message and exit the shell. */
          perror("Internal error: cannot wait for child.");
          std::cout << waitpidResult;
          exit(1);
        }
      }
    }
  }
}

void koopa::launch_process(process p) {
  /* 
   * Execute a program whose command is p.argv[0] and whose arguments 
   * are the rest of the p.argv array. Then, print errors, if any, to
   * std::cout, and exit the program.
   */
  execvp(p.argv[0], p.argv);
  perror(p.argv[0]);
  exit(1);
}
