// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#define __SYSCALL_LL_E(x) (x)
#define __SYSCALL_LL_O(x) (x)

#include "libc.h"

static __inline  __attribute__((always_inline)) long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
    register long r8 __asm__("r8") = a4;
    register long r9 __asm__("r9") = a5;
    register long r13 __asm__("r13") = a6;
    register long r12 __asm__("r11") = (long)libc.visorcall;

    unsigned long ret;
    __asm__ __volatile__("lea return_target%=(%%rip), %%r12\n\t" // load return address into r12
                         "mov 0(%%r11), %%r10d\n\t"
                         "add $0x05e1f00d, %%r10d\n\t"
                         "jnz wrong_target%=\n\t"
                         "jmp *%%r11\n\t"
                         "wrong_target%=:\n\t"
                         "int3\n\t"
                         "return_target%=:\n\t"
                         "endbr64\n\t": "=a"(ret) :
                         "r"(r12), "D"(n), "S"(a1), "d"(a2), "c"(a3), "r"(r8), "r"(r9), "r"(r13) : "rbx", "r10", "r12", "flags", "memory");
    return ret;
}

static __inline __attribute__((always_inline)) long __syscall0(long n)
{
    return __syscall6(n, 0, 0, 0, 0, 0, 0);
}

static __inline __attribute__((always_inline)) long __syscall1(long n, long a1)
{
    return __syscall6(n, a1, 0, 0, 0, 0, 0);
}

static __inline __attribute__((always_inline)) long __syscall2(long n, long a1, long a2)
{
    return __syscall6(n, a1, a2, 0, 0, 0, 0);
}

static __inline __attribute__((always_inline)) long __syscall3(long n, long a1, long a2, long a3)
{
    return __syscall6(n, a1, a2, a3, 0, 0, 0);
}

static __inline __attribute__((always_inline)) long __syscall4(long n, long a1, long a2, long a3, long a4)
{
    return __syscall6(n, a1, a2, a3, a4, 0, 0);
}

static __inline __attribute__((always_inline)) long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
    return __syscall6(n, a1, a2, a3, a4, a5, 0);
}

// removed VDSO definitions
#define GRAALOS

#define IPC_64 0
