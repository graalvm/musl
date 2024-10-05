// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#include "syscall.h"

// __syscall_cp_asm is not needed in GraalOS


_Noreturn void __visorcall() {
    register long r11 __asm__("r11") = (long)libc.visorcall;

    __asm__ __volatile__(
    "mov 0(%%r11), %%r10d\n\t"
    "add $0x05e1f00d, %%r10d\n\t"
    "jnz wrong_target%=\n\t"
    "jmp *%%r11\n\t"
    "wrong_target%=:\n\t"
    "int3\n\t"
    :: "r"(r11));
}
