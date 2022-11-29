// Copyright (c) 2022 Oracle and/or its affiliates. All rights reserved.
#include "pthread_impl.h"
#include "syscall_arch.h"

int __set_thread_area(void *p)
{
    // this call sets this thread's fs segment register to p
    return graal_syscall2(SYS_arch_prctl, SET_FS, p);
}
