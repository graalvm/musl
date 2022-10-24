#include "graal_syscall.h"


static syscall_handler_t graal_syscall_handler;
static void *graal_context;

void graal_syscall_handler_set(syscall_handler_t handler, void *context) {
    graal_syscall_handler = handler;
    graal_context = context;
}

long graal_syscall(long n, long a1, long a2, long a3, long a4, long a5, long a6){
    if (graal_syscall_handler != 0) {
        return (graal_syscall_handler)(graal_context, n, a1, a2, a3, a4, a5, a6);
    } else {
        unsigned long ret;
        register long r10 __asm__("r10") = a4;
        register long r8 __asm__("r8") = a5;
        register long r9 __asm__("r9") = a6;
        __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
                                                  "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
        return ret;
    }

    return 0;
}
