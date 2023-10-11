// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#include "pthread_impl.h"
#include "graal_syscall.h"

long graal_syscall(long n, long a1, long a2, long a3, long a4, long a5, long a6) {
    pthread_t self = pthread_self();
    return (*self->syscall)(n, a1, a2, a3, a4, a5, a6);
}
