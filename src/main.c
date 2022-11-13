#include <stdlib.h>
#include "shell.h"
#include <signal.h>

static shell_t shell;

void handle_sigchld(int sig) {
    pid_t p = wait(NULL);
    // print prompt again if process was FG, BG shouldn't write to stdout so no need
    job_status s = shell_remove_job(&shell, p);
    printf("finished job status: %d\n", s);
    if (s == BG) {
        shell_print_prompt();
    }
}

int main( int argc, char **argv )
{

    signal(SIGCHLD, handle_sigchld);

    shell_init( &shell );
    shell_run( &shell );
    shell_free( &shell );
    return ( EXIT_SUCCESS );
    (void)argc;
    (void)argv;
}
