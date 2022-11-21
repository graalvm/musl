#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include "libc.h"
#include "pthread_impl.h"
#include "initial_thread.h"

void graal_init_after_clone(struct musl_loader *ml);
void *graal_init_before_clone(struct musl_loader *ml);

static int graal_start(void *arg)
{
    struct musl_loader *ml = arg;

    graal_init_after_clone(ml);

    if (ml->env) {
        __environ = ml->env;
    } else {
        __environ = calloc(1, sizeof(char *));
    }


    exit((*ml->entry)(ml->argc, ml->argv));

    return 0;
}

long graalos_initial_thread(struct musl_loader *ml)
{
    unsigned char *stack = 0;
    unsigned flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND
        | CLONE_THREAD | CLONE_SYSVSEM | CLONE_SETTLS
        | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID | CLONE_DETACHED;
    struct pthread *self = graal_init_before_clone(ml);


    /*
     * New pthreads do somethings that we aren't going to do here.
     * 1. Initialized file locks.  The second thread will do that
     *    via pthread_create.
     * 2. Setup pthread_keys.  pthread_create_key or pthread_create
     *    for subsequent threads will set that up.
     */

    stack = ml->stack;
    stack += ml->stack_size;

    return  (ml->clone_func)(graal_start, stack, flags, ml, &ml->initial_tid, TP_ADJ(self), &__thread_list_lock);

}
    
     


