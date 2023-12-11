// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#ifndef GRAALOS_PTHREAD_H
#define GRAALOS_PTHREAD_H

#include <fcntl.h>
#include <locale.h>
#include <stddef.h>
#include <stdint.h>

#include "musl_types.h"

#ifndef TP_OFFSET
#define TP_OFFSET 0
#endif

#ifndef DTP_OFFSET
#define DTP_OFFSET 0
#endif

#ifdef __cplusplus
#define GRAALOS
namespace musl {
#endif


struct clone_params_t {
    start_fn_t start;
    thread_fn_t func;
    // int     flags;        -- removed because no user control of this is
    // supported
    void *arg;
    void *stack;
    void *tls;
    pid_t *ptid;
    pid_t *ctid;
};

#ifdef __cplusplus
} // namespace musl
#endif

#endif // GRAALOS_PTHREAD_H
