// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#define __SYSCALL_LL_E(x) (x)
#define __SYSCALL_LL_O(x) (x)

#include "libc.h"

static __inline long __syscall0(long n)
{
        return libc.visorcall(n, 0, 0, 0, 0, 0, 0);
}

static __inline long __syscall1(long n, long a1)
{
        return libc.visorcall(n, a1, 0, 0, 0, 0, 0);
}

static __inline long __syscall2(long n, long a1, long a2)
{
        return libc.visorcall(n, a1, a2, 0, 0, 0, 0);
}

static __inline long __syscall3(long n, long a1, long a2, long a3)
{
        return libc.visorcall(n, a1, a2, a3, 0, 0, 0);
}

static __inline long __syscall4(long n, long a1, long a2, long a3, long a4)
{
        return libc.visorcall(n, a1, a2, a3, a4, 0, 0);
}

static __inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
        return libc.visorcall(n, a1, a2, a3, a4, a5, 0);
}

static __inline long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
        return libc.visorcall(n, a1, a2, a3, a4, a5, a6);
}

// removed VDSO definitions
#define GRAALOS

#define IPC_64 0
