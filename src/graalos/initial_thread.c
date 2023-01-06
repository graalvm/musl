// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
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

    // give graalos a chance to assert its control over scheduling and priority of this isolate thread and its eventual children
    struct sched_param param;
    pthread_t t = __pthread_self();
	__syscall(SYS_sched_getparam, t->tid, &param);
    __syscall(SYS_sched_setparam, t->tid, &param);

    exit((*ml->entry)(ml->argc, ml->argv));

    return 0;
}

int graalos_initial_thread(struct musl_loader *ml, void* tp)
{
    struct pthread *self = graal_init_before_clone(ml);

    /*
     * New pthreads do somethings that we aren't going to do here.
     * 1. Initialized file locks.  The second thread will do that
     *    via pthread_create.
     * 2. Setup pthread_keys.  pthread_create_key or pthread_create
     *    for subsequent threads will set that up.
     */

    unsigned char *stack = (unsigned char*)ml->stack + ml->stack_size;
    return  (ml->clone_func)(graal_start, stack, ml, &ml->initial_tid, TP_ADJ(self), &__thread_list_lock, tp);
}
    
     


