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
    s->jobs_capacity = 100;
    s->jobs_size   = 0;
    s->buffer      = malloc( s->buffer_size * sizeof( char ) );
    s->jobs        = malloc( s->jobs_capacity * sizeof ( job_t ) );
}

int shell_add_job( shell_t *s, job_t job ) {
    if (s->jobs_size +1 >= s->jobs_capacity) {
        printf("ERROR: Cannot add any more jobs !");
        return -1;
    }

    int i=0;
    while (i < s->jobs_capacity) {
        if (s->jobs[i].name[0] == '\0') {
            s->jobs[i] = job;
            s->jobs_size++;
            return i;
        }
        i++;
    }
    printf("ERROR: Couldn't find any space to add new job to jobs !");
    return -1;
}

void shell_display_jobs( shell_t *s ) {
    printf("PID     NAME\n");

    int i=0;
    while (i < s->jobs_capacity) {
        if (s->jobs[i].name[0] != '\0') {
            printf("%d      %s\n", s->jobs[i].pid, &s->jobs[i].name[0]);
        }
        i++;
    }
}

void shell_remove_job( shell_t *s, int job_id) {
    if (job_id > s->jobs_size) {
        printf("ERROR: Cannot remove invalid job_id");
        return;
    }
    s->jobs[job_id].name[0] = '\0';
    s->jobs[job_id].pid = -1;
}

void print_prompt() {
    printf("$");

    printf(" ");
}

void shell_run( shell_t *s ) {
    s->running = true;

    while (s->running) {
        print_prompt();
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
    struct StringVector tokens = split_line(s->buffer);
    int nb_tokens = string_vector_size(&tokens);

    if (nb_tokens == 0) {
        printf("-> Nothing to do !\n");
    } else {
        char *name = string_vector_get(&tokens, 0);
        Action action = get_action(name);
        action(s, & tokens);

        string_vector_free(& tokens);
    }
}
