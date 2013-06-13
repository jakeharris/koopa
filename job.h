#ifndef JOB_H
#define JOB_H

#include "process.h"

#include <sys/types.h>
#include <termios.h>

namespace koopa {
  /* A job is a pipeline of processes.  */
  typedef struct job {
     struct job *next;           /* next active job */
     char *command;              /* command line, used for messages */
     process *first_process;     /* list of processes in this job */
     pid_t pgid;                 /* process group ID */
     char notified;              /* true if user told about stopped job */
     struct termios tmodes;      /* saved terminal modes */
     int stdin, stdout, stderr;  /* standard i/o channels */
  } job;
}

#endif