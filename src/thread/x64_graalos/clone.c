// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#define _GNU_SOURCE
#include <stdarg.h>
#include <unistd.h>
#include <sched.h>
#include "pthread_impl.h"
#include "syscall.h"
#include "syscall_internal.h"

typedef int (*thread_fn_t)(void* arg);
typedef void (*start_fn_t)(thread_fn_t fn, void* arg);

struct clone_params_t {
  start_fn_t start;
  thread_fn_t func;
  // int     flags;
  void *arg;
  void *stack;
  void *tls;
  pid_t *ptid;
  pid_t *ctid;
};

void graalos_start(thread_fn_t fn, void *arg)
{
    int res = fn(arg);
    __syscall1(SYS_exit, res);
    __asm__ __volatile__("int3" :);
}

int __clone(int (*func)(void *), void *stack, int flags, void *arg, ...)
{
    struct clone_params_t params;
    params.start = &graalos_start;
    params.func = func;
    params.stack = stack;
    //params.flags = flags;
    params.arg = arg;

    va_list ap;
    va_start(ap, arg);
    params.ptid = va_arg(ap, pid_t *);
    params.tls  = va_arg(ap, void *);
    params.ctid = va_arg(ap, pid_t *);
    va_end(ap);

    return __syscall_ret(__syscall1(__VISORCALL_clone, &params));
}
