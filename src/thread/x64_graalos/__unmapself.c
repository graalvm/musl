// Copyright (c) 2022 Oracle and/or its affiliates. All rights reserved.
#include "pthread_impl.h"
#include "syscall_arch.h"

int __unmapself(void *base, size_t size)
{
    // this call terminates the calling thread and unmaps its stack (specified by <base,size>)
    return graal_syscall1(__VISORCALL_UNMAPSELF, base, size);
}

