//
// Created by Leiko Lei on 18/10/2022.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef BRU_H_SHELL_H
#define BRU_H_SHELL_H

typedef struct job_t {
    pid_t  pid;
    char   name[50];
} job_t;

typedef struct shell_t {
    bool   running;
    int    line_number;
    char   *buffer;
    size_t buffer_size;
    job_t  *jobs;
    int    jobs_size;
    int    jobs_capacity;
} shell_t;

void shell_init( shell_t *s );
void shell_run( shell_t *s );
void shell_free( shell_t *s );

void shell_read_line( shell_t *s );
void shell_execute_line( shell_t *s );

int shell_add_job( shell_t *s, job_t job );
void shell_remove_job( shell_t *s, int job_id );
void shell_display_jobs( shell_t *s );

#endif //BRU_H_SHELL_H
