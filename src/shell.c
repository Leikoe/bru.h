#include <stdio.h>
#include <sys/types.h>

#include "shell.h"

void shell_init( struct Shell *s ) {
    s->running     = false;
    s->line_number = 0;
    s->buffer_size = 16;
    s->buffer      = malloc( s->buffer_size * sizeof( char ) );
}

void shell_run( struct Shell *s ) {
    s->running = true;

    while (s->running) {
        shell_read_line(s);
        shell_execute_line(s);
    }
}

void shell_free( struct Shell *s ) {
    free( s->buffer );
}

void shell_read_line( struct Shell *s ) {
    // man 3 getline
    size_t linelen;
    linelen = getline( &(s->buffer), &( s->buffer_size ), stdin );
}

void shell_execute_line( struct Shell *s ) {
    printf("%s", s->buffer);
}
