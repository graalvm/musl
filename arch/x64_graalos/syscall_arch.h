// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#define __SYSCALL_LL_E(x) (x)
#define __SYSCALL_LL_O(x) (x)

#include "libc.h"

// load return address into r12
#define SYSCALL_ASM_SEQ                     \
    "lea return_target%=(%%rip), %%r12\n\t" \
    "mov 0(%%r11), %%r10d\n\t"              \
    "add $0x05e1f00d, %%r10d\n\t"           \
    "jnz wrong_target%=\n\t"                \
    "jmp *%%r11\n\t"                        \
    "wrong_target%=:\n\t"                   \
    "int3\n\t"                              \
    "return_target%=:\n\t"                  \
    "endbr64\n\t"

#define SYSCALL_CLOBBER_COMMON "r10", "r12", "r13", "r14", "memory"

static __inline  __attribute__((always_inline)) long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
    register long r8 __asm__("r8") = a5;
    register long r9 __asm__("r9") = a6;
    register long r11 __asm__("r11") = (long)libc.visorcall;

    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ: "=a"(ret), "=r"(r11) :
                         "r"(r11), "a"(n), "D"(a1), "S"(a2), "d"(a3), "c"(a4), "r"(r8), "r"(r9) : SYSCALL_CLOBBER_COMMON);
    return ret;
}

static __inline __attribute__((always_inline)) long __syscall0(long n)
{
    register long r11 __asm__("r11") = (long)libc.visorcall;

    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ : "=a"(ret), "=r"(r11) :
                         "r"(r11), "a"(n) : SYSCALL_CLOBBER_COMMON);
    return ret;
}

static __inline __attribute__((always_inline)) long __syscall1(long n, long a1)
{
    register long r11 __asm__("r11") = (long)libc.visorcall;

    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ : "=a"(ret), "=r"(r11) :
                         "r"(r11), "a"(n), "D"(a1) : SYSCALL_CLOBBER_COMMON);
    return ret;
}

static __inline __attribute__((always_inline)) long __syscall2(long n, long a1, long a2)
{
    return __syscall6(n, a1, a2, 0, 0, 0, 0);
}

static __inline __attribute__((always_inline)) long __syscall3(long n, long a1, long a2, long a3)
{
    register long r11 __asm__("r11") = (long)libc.visorcall;

    unsigned long ret;
    __asm__ __volatile__(SYSCALL_ASM_SEQ : "=a"(ret), "=r"(r11) :
                         "r"(r11), "a"(n), "D"(a1), "S"(a2), "d"(a3) : SYSCALL_CLOBBER_COMMON);
    return ret;
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
