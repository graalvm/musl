// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#ifndef GRAALOS_PTHREAD_H
#define GRAALOS_PTHREAD_H

#include <fcntl.h>
#include <locale.h>
#include <stddef.h>
#include <stdint.h>
#include <signal.h>

#include "musl_types.h"

#ifndef TP_OFFSET
#define TP_OFFSET 0
#endif

#ifndef DTP_OFFSET
#define DTP_OFFSET 0
#endif

#ifdef __cplusplus
#ifndef GRAALOS
#define GRAALOS
#endif
namespace musl {
#endif


struct clone_params_t {
    thread_fn_t func;
    uint64_t flags;
    void *arg;
    void *stack;
    void *tls;
    pid_t *ptid;
    pid_t *ctid;
};

struct sigcall_res {
    uint64_t  handler;
    uint64_t  syscall_number;
    uint64_t  syscall_result;
    uint64_t  sigmask;
    siginfo_t info;
};

#ifdef __cplusplus
} // namespace musl
#endif

#endif // GRAALOS_PTHREAD_H
