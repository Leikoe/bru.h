#include <stdlib.h>
#include "shell.h"

int main( int argc, char **argv )
{
    shell_t shell;
    shell_init( &shell );
    shell_run( &shell );
    shell_free( &shell );
    return ( EXIT_SUCCESS );
    (void)argc;
    (void)argv;
}
