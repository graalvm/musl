// Copyright (c) 2025 Oracle and/or its affiliates. All rights reserved.
#define _GNU_SOURCE

#include <sched.h>
#include <signal.h>
#include "include/graalos/musl_thread.h"
#include "vfork_internal.h"

hidden const uint64_t __vfork_clone_flags = CLONE_VM | CLONE_VFORK | SIGCHLD;

_Static_assert(SIZEOF_CLONE_PARAMS == sizeof(struct clone_params_t), "");
_Static_assert(OFFSETOF_CLONE_PARAMS_FUNC == offsetof(struct clone_params_t, func), "");
_Static_assert(OFFSETOF_CLONE_PARAMS_FLAGS == offsetof(struct clone_params_t, flags), "");
_Static_assert(OFFSETOF_CLONE_PARAMS_STACK == offsetof(struct clone_params_t, stack), "");
