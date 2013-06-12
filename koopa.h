using namespace koopa;

/* Set koopa as current shell.
 * Take control of terminal.
 * Turn off job control signals.
 * Make a process group.
 */
void init_shell();

/* Divide args into job(s).
 * Execute args.
 */
void launch_args(args);

/* Divide job into process(es).
 * Execute job.
 */
void launch_job(job);

/* Execute process. */
void launch_process(process);

/* Set default shell as current shell.
 * Relinquish control of terminal.
 */
/* void quit_shell(); */