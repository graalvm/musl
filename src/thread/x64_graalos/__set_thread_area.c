// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#include "pthread_impl.h"

// from toolchain's asm/prctl.h which we cannot directly include from musl
#define SET_FS  0x1002

int __set_thread_area(void *p)
{
    // this call sets this thread's fs segment register to p
    return __syscall2(SYS_arch_prctl, SET_FS, p);
}
