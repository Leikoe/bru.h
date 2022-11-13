#include "actions.h"
#include "shell.h"
#include "string_vector.h"
#include <unistd.h>
#include "string.h"
#include "stdlib.h"
#include <stdbool.h>
#include <fcntl.h>


static void do_exit(shell_t *this, const struct StringVector *args)
{
    this->running = false;
    printf("quitting shell..\n");
    args;
}

static void do_cd(shell_t *this, const struct StringVector *args)
{
    char *dst_dir = string_vector_get(args, 1);
    if (dst_dir == NULL)
    {
        dst_dir = getenv("HOME");
    }
    chdir(dst_dir);
}

static void do_pwd(shell_t *this, const struct StringVector *args)
{
    pid_t p = fork();
    if (p == 0) {
        execvp("pwd", args->strings);
        exit (EXIT_SUCCESS);
    }
    wait(&p);
}

static void do_help(shell_t *this, const struct StringVector *args)
{
    printf("-> commands: exit, cd, help, ?\n");
    args;
}

pid_t start(const char *file, char * const *args, bool blocking) {
    pid_t p = fork();
    if (p == 0) {
        execvp(file, args);
        exit(EXIT_SUCCESS);
    }
    int pid = p;
    if (blocking) {
        wait(&p);
    }
    return pid;
}

static void do_system(shell_t *this, const struct StringVector *args)
{
    char *file = string_vector_get(args, 1);
    // this is ok because size is always at least one when do_{something} is called
    char *last_arg = string_vector_get(args, string_vector_size(args) - 1);

    pid_t p;

    // insane fix for the execvp command
    string_vector_add(args, NULL, NULL);
    if (strcmp(last_arg, "&") == 0) {
        // remove the & from the args
        args->strings[string_vector_size(args) -2] = NULL;

        // TODO: add to a jobs datastructure
        p = start(file, args->strings + 1, false);

        job_t job = {
                .pid = p,
                .status = BG,
        };

        strcpy(job.name, file);

        shell_add_job(this, job);
        printf("[+] %d\n", p);
    } else {
        p = start(file, args->strings + 1, true);
    }
}

static void do_unknown(shell_t *this, const struct StringVector *args)
{
    printf("unknown command !\n");
}

static void do_jobs(shell_t *this, const struct StringVector *args)
{
    shell_display_jobs(this);
}


static void do_list(shell_t *this, const struct StringVector *args)
{
    pid_t p = fork();
    if (p == 0) {
        execvp("ls", args->strings);
        exit (EXIT_SUCCESS);
    }
    wait(&p);
}

static void do_createDir(shell_t *this, const struct StringVector *args)
{
    char *dir = string_vector_get(args, 1);
    if (dir == NULL)
    {
        printf("missing argument !\n");
        return;
    }
    pid_t p = fork();
    if (p == 0) {
        execvp("mkdir", args -> strings);
        exit (EXIT_SUCCESS);
    }
    wait(&p);
}

static void do_removeDir(shell_t *this, const struct StringVector *args)
{
    char *dir = string_vector_get(args, 1);
    if (dir == NULL)
    {
        printf("missing argument !\n");
        return;
    }
    pid_t p = fork();
    if (p == 0) {
        execvp("rmdir", args -> strings);
        exit (EXIT_SUCCESS);
    }
    wait(&p);
}

static void do_rm(shell_t *this, const struct StringVector *args)
{
    char *file = string_vector_get(args, 1);
    if (file == NULL)
    {
        printf("missing argument !\n");
        return;
    }
    pid_t p = fork();
    if (p == 0) {
        execvp("rm", args -> strings);
        exit (EXIT_SUCCESS);
    }
    wait(&p);
}

static void do_cp(shell_t *this, const struct StringVector *args)
{
    char *file = string_vector_get(args, 1);
    char *dst = string_vector_get(args, 2);
    if (file == NULL || dst == NULL)
    {
        printf("missing argument !\n");
        return;
    }
    pid_t p = fork();
    if (p == 0) {
        execvp("cp", args -> strings);
        exit (EXIT_SUCCESS);
    }
    wait(&p);
}

static void do_move(shell_t *this, const struct StringVector *args)
{
    char *file = string_vector_get(args, 1);
    char *dst = string_vector_get(args, 2);
    if (file == NULL || dst == NULL)
    {
        printf("missing argument !\n");
        return;
    }
    pid_t p = fork();
    if (p == 0) {
        execvp("mv", args -> strings);
        exit (EXIT_SUCCESS);
    }
    wait(&p);
}

static struct {
    const char *name;
    Action action;
} actions[] = {
        { .name = "exit", .action = do_exit},
        { .name = "cd",   .action = do_cd},
        { .name = "pwd",  .action = do_pwd},
        { .name = "help", .action = do_help},
        { .name = "jobs", .action = do_jobs},
        { .name = "?",    .action = do_help},
        { .name = "!",    .action = do_system},
        { .name = "ls",   .action = do_list},
        { .name = "mkdir",.action = do_createDir},
        { .name = "rmdir",.action = do_removeDir},
        { .name = "rm",   .action = do_rm},
        { .name = "cp",   .action = do_cp},
        { .name = "mv",   .action = do_move},
        { .name = NULL,   .action = do_unknown}
};

Action get_action(char * name)
{
    int i = 0;
    while (actions[i].name != NULL
           && strcmp(actions[i].name, name) != 0) {
        i++;
    }
    return actions[i].action;
}
