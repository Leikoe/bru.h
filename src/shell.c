#include <stdio.h>
#include <sys/types.h>
#include "string_vector.h"
#include "shell.h"
#include <string.h>
#include <unistd.h>


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
    size_t linelen;
    linelen = getline( &(s->buffer), &( s->buffer_size ), stdin );
}

void shell_execute_line( shell_t *s ) {
    struct StringVector sv = split_line(s->buffer);
    char *cmd = string_vector_get(&sv,0);

    if (cmd == NULL) {
        return;
    }

    if (strcmp(cmd, "help") == 0) {
        printf("type exit to quit");

    } else if (strcmp(cmd, "exit") == 0) {
        s->running = false;
        printf("quitting shell..");

    } else if (strcmp(cmd, "!") == 0) {
        char *shell_env_var = getenv("SHELL");
        char *command = malloc(sizeof(char) * string_vector_size(&sv));
        sprintf(command, "%s %s", shell_env_var, (char *)(s->buffer +1));
        system(command);

    } else if (strcmp(cmd, "cd") == 0) {
        char *dst_dir = string_vector_get(&sv, 1);
        chdir(dst_dir);

    } else if (strcmp(cmd, "ls") == 0) {
        system(s->buffer);

    } else if (strcmp(cmd, "clear") == 0) {
        system("clear");

    } else {
        printf("unknown command");
    }

    printf("\n");
}
