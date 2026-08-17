// Copyright (c) 2022-2026 Oracle and/or its affiliates. All rights reserved.
#include "pthread_impl.h"
#include "syscall_internal.h"

_Noreturn void __unmapself(void *base, size_t size)
{
    __syscall2(SYS_munmap, base, size);
    while (__syscall1(SYS_exit, 0) == -ERESTARTSYS) {
    }
    __asm__ __volatile__("int3");

    __builtin_unreachable();
}
