// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#include "pthread_impl.h"

void __unmapself(void *base, size_t size)
{
    // this call terminates the calling thread and unmaps its stack (specified by <base,size>)
    __syscall2(__VISORCALL_unmapself, base, size);
}

