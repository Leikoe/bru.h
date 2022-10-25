#include "actions.h"
#include "shell.h"
#include "string_vector.h"
#include <unistd.h>
#include "string.h"
#include "stdlib.h"

static void do_exit(shell_t *this, const struct StringVector *args)
{
    this->running = false;
    printf("quitting shell..");
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
    args;
}

static void do_pwd(shell_t *this, const struct StringVector *args)
{
    system("pwd");
    args;
}

static void do_help(shell_t *this, const struct StringVector *args)
{
    printf("-> commands: exit, cd, help, ?.\n");
    args;
}

static void do_system(shell_t *this, const struct StringVector *args)
{
    char *shell_env_var = getenv("SHELL");
    char *command = malloc(sizeof(char) * string_vector_size(args));
    sprintf(command, "%s %s", shell_env_var, (char *)(this->buffer +1));
    system(command);
    args;
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


static void do_execute(shell_t *this, const struct StringVector *args)
{
    char *file = string_vector_get(args, 0);
//    char *args_ = string_vector_slice(args, 1);
//    if (file != NULL && fork()==0) {
//        execlp(file, args);
//    }
}

static struct {
    const char *name;
    Action action;
} actions[] = {
        { .name = "exit", .action = do_exit},
        { .name = "cd",   .action = do_cd},
        { .name = "pwd",  .action = do_pwd},
        { .name = "help", .action = do_help},
        { .name = "?",    .action = do_help},
        { .name = "!",    .action = do_system},
        { .name = "ls",   .action = do_list},
        { .name = NULL,   .action = do_execute}
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
