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

void shell_add_job( shell_t *s, job_t job ) {
    if (s->jobs_size +1 >= s->jobs_capacity) {
        printf("ERROR: Cannot add any more jobs !");
        return;
    }

    int i=0;
    while (i < s->jobs_capacity) {
        if (s->jobs[i].status == UNKNOWN) {
            s->jobs[i] = job;
            s->jobs_size++;

            printf("allocated job (%d) \n", i);
            return;
        }
        i++;
    }
    printf("ERROR: Couldn't find any space to add new job to jobs !");
}

void shell_display_jobs( shell_t *s ) {
    printf("PID     NAME\n");

    int i=0;
    while (i < s->jobs_capacity) {
        if (s->jobs[i].status != UNKNOWN) {
            printf("%d      %s\n", s->jobs[i].pid, &s->jobs[i].name[0]);
        }
        i++;
    }
}


// returns removed job's status
job_status shell_remove_job( shell_t *s, pid_t pid ) {
    int i=0;
    while (i< s->jobs_capacity) {

        if (s->jobs[i].pid == pid && s->jobs[i].status != UNKNOWN) {
            job_status status = s->jobs[i].status;

            s->jobs[i].status = UNKNOWN;
            s->jobs[i].name[0] = '\0';
            s->jobs[i].pid = -1;

            printf("\n[-] %d\n", pid);

            return status;
        }
        i++;
    }

    return UNKNOWN;
}

void shell_print_prompt() {
    printf("$");

    printf(" ");
    fflush(stdout);
}

void shell_run( shell_t *s ) {
    s->running = true;

    while (s->running) {
        shell_print_prompt();
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
    size_t linelen = -1;
    while (linelen == -1) {
	clearerr(stdin);
        linelen = getline( &(s->buffer), &( s->buffer_size ), stdin );
    }
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
    }

    string_vector_free(&tokens);
}
