#include <stdio.h>
#include <sys/types.h>
#include "string_vector.h"
#include "shell.h"
#include <string.h>
#include <unistd.h>
#include "actions.h"

void shell_init( shell_t *s ) {
    s->running     = false;
    s->line_number = 0;
    s->buffer_size = 16;
    s->buffer      = malloc( s->buffer_size * sizeof( char ) );
}

void shell_run( shell_t *s ) {
    s->running = true;

    while (s->running) {
        printf("$ ");
        shell_read_line(s);
        shell_execute_line(s);
    }
}

void shell_free( shell_t *s ) {
    free( s->buffer );
}

void shell_read_line( shell_t *s ) {
    // man 3 getline
    s->line_number++;
    size_t linelen;
    linelen = getline( &(s->buffer), &( s->buffer_size ), stdin );
}

void shell_execute_line( shell_t *s ) {
    struct StringVector sv = split_line(s->buffer);
    char *cmd = string_vector_get(&sv,0);

    if (cmd == NULL) {
        return;
    }

    Action a = get_action(cmd);
    a(s, &sv);

    printf("\n");
    string_vector_free(&sv);
}
