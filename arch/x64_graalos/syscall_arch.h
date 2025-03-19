// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#define __SYSCALL_LL_E(x) (x)
#define __SYSCALL_LL_O(x) (x)

#include "libc.h"


// load return address into r12
#define SYSCALL_ASM_SEQ                     \
    "lea return_target%=(%%rip), %%r12\n\t" \
    "jmp __visorcall\n\t"                   \
    "return_target%=:\n\t"                  \
    "endbr64\n\t"

#define SYSCALL_CLOBBER_COMMON "r10", "r11", "r12", "r13", "memory"

static __inline __attribute__((always_inline)) long __syscall0(long n)
{
    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ : "=a"(ret) :
                         "a"(n) : SYSCALL_CLOBBER_COMMON);
    return ret;
}

static __inline __attribute__((always_inline)) long __syscall1(long n, long a1)
{
    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ : "=a"(ret) :
                         "a"(n), "D"(a1) : SYSCALL_CLOBBER_COMMON);
    return ret;
}

static __inline __attribute__((always_inline)) long __syscall2(long n, long a1, long a2)
{
    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ: "=a"(ret) :
                         "a"(n), "D"(a1), "S"(a2) : SYSCALL_CLOBBER_COMMON);
    return ret;
}

static __inline __attribute__((always_inline)) long __syscall3(long n, long a1, long a2, long a3)
{
    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ : "=a"(ret) :
                         "a"(n), "D"(a1), "S"(a2), "d"(a3) : SYSCALL_CLOBBER_COMMON);
    return ret;
}

static __inline __attribute__((always_inline)) long __syscall4(long n, long a1, long a2, long a3, long a4)
{
    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ: "=a"(ret) :
                         "a"(n), "D"(a1), "S"(a2), "d"(a3), "c"(a4) : SYSCALL_CLOBBER_COMMON);
    return ret;
}

static __inline __attribute__((always_inline)) long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
    register long r8 __asm__("r8") = a5;

    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ: "=a"(ret) :
                         "a"(n), "D"(a1), "S"(a2), "d"(a3), "c"(a4), "r"(r8) : SYSCALL_CLOBBER_COMMON);
    return ret;
}

static __inline  __attribute__((always_inline)) long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
    register long r8 __asm__("r8") = a5;
    register long r9 __asm__("r9") = a6;

    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ: "=a"(ret) :
                         "a"(n), "D"(a1), "S"(a2), "d"(a3), "c"(a4), "r"(r8), "r"(r9) : SYSCALL_CLOBBER_COMMON);
    return ret;
}

static __inline long __syscall_direct()
{
    __asm__ __volatile__("mov %%r10, %%rcx"
                       :
                       :
                       : "rcx");

    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ: "=a"(ret));
    return ret;
}

// removed VDSO definitions
#define GRAALOS

#define IPC_64 0
