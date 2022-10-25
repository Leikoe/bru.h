#ifndef BRU_H_ACTIONS_H
#define BRU_H_ACTIONS_H

#include "string_vector.h"
#include "shell.h"

typedef void (*Action) (
        shell_t *,
        const struct StringVector *
);

Action get_action(char * name);

#endif //BRU_H_ACTIONS_H
