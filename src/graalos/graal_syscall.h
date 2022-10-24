#ifndef __GRAAL_SYSCALL_H
#define __GRAAL_SYSCALL_H

typedef long (*syscall_handler_t)(void *ctx, long n, long a1, long a2, long a3, long a4, long a5, long a6);

void graal_syscall_handler_set(syscall_handler_t handler, void *context);
long graal_syscall(long n, long a1, long a2, long a3, long a4, long a5, long a6);

#endif /* __GRAAL_SYSCALL_H */
