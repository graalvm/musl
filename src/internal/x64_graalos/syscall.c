// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#include "graal_syscall.h"
#include "pthread_impl.h"
#include "reloc.h"

long graal_syscall(long n, long a1, long a2, long a3, long a4, long a5, long a6) {
    pthread_t self = pthread_self();

    long res = (*self->syscall)(n, a1, a2, a3, a4, a5, a6);
    cfi_branch_target();

    return res;
}
