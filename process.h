#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

namespace koopa {
  /* currently using the GNU basic shell tutorial to get through the baby steps */

  typedef struct process {
     struct process *next;  	/* next process in pipeline */
     char **argv;			/* the arguments for execution */ 
     pid_t pid;			/* process ID */
     char completed;		/* true if process has completed */
     char stopped;		/* true if process has stopped */
     int status;			/* reported status value */
  } process;
}

#endif
