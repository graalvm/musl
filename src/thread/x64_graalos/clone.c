// Copyright (c) 2022 Oracle and/or its affiliates. All rights reserved.
#define _GNU_SOURCE
#include <stdarg.h>
#include <unistd.h>
#include <sched.h>
#include "pthread_impl.h"
#include "syscall.h"

struct clone_params_t
{
    int   (*func)(void *);
    int     flags;
    void*   arg;
    void*   stack;
    void*   tls;
    pid_t*  ptid;
    pid_t*  ctid;
};


int __clone(int (*func)(void *), void *stack, int flags, void *arg, ...)
{
    struct clone_params_t params;
    params.func = func;
    params.flags = flags;
    params.arg = arg;
    params.stack = stack;

    va_list ap;
    va_start(ap, arg);
    params.ptid = va_arg(ap, pid_t *);
    params.tls  = va_arg(ap, void *);
    params.ctid = va_arg(ap, pid_t *);
    va_end(ap);

    return __syscall_ret(__syscall1(__VISORCALL_CLONE, &params));
}
