// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#define _GNU_SOURCE
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <sched.h>
#include "pthread_impl.h"
#include "syscall.h"
#include "syscall_internal.h"
// #include "include/graalos/musl_thread.h"

typedef int (*thread_fn_t)(void *arg);

struct clone_params_t {
    thread_fn_t func;
    uint64_t flags;
    void *arg;
    void *stack;
    void *tls;
    pid_t *ptid;
    pid_t *ctid;
};

int __clone(int (*func)(void *), void *stack, int flags, void *arg, ...)
{
    // Align the stack pointer to 16 bytes
    uint64_t * wstack = (uint64_t*)((uint64_t)stack & -16);

    struct clone_params_t params;
    params.func = func;
    params.stack = wstack;
    params.flags = flags;
    params.arg = arg;

    va_list ap;
    va_start(ap, arg);
    params.ptid = va_arg(ap, pid_t *);
    params.tls  = va_arg(ap, void *);
    params.ctid = va_arg(ap, pid_t *);
    va_end(ap);

    return __syscall_ret(__syscall1(__VISORCALL_clone, &params));
}
