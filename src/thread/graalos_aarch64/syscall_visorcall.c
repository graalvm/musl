#include "syscall.h"
#include "libc.h"
#include <stddef.h>

_Noreturn void __visorcall(void) {
    // we have to be careful here because on aarch64
    // we cannot directly move libc.visorcall into x21 without an extra register
    // by default the compiler chooses to clobber x8 (syscall no)
    // instead we could manually spill x8, load visorcall into x8, and restore x8, then branch
    // but this messes up for pthread_exit which already unmaps the stack before doing syscall(SYS_exit)
    // because the stack is no longer available
    // instead, use x23 as temporary register, which is marked as clobber for syscalls
    // now, load __libc.visorcall and jump there
    __asm__ __volatile__ (
        "adrp   x23, :got:__libc\n"
        "ldr    x23, [x23, #:got_lo12:__libc]\n"
        "ldr    x21, [x23, %c[offset]]\n"
        "br     x21\n"
        :
        : [offset] "i" (offsetof(struct __libc, visorcall))
        : "memory" // dont mark x21 as clobber so that it doesnt get pushed to stack, here
    );
    __builtin_unreachable();
}
