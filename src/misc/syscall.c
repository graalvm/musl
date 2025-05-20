#define _BSD_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include "syscall.h"
#include <stdarg.h>
#include "stdio.h"

#undef syscall

#define VISORCALL_mmap_fixed_offset          ((1ul << 63) | (10ul))

#ifdef DUMP_SYSCALLS
void dump_syscall(long n, long a, long b, long c, long d, long e, long f) {
	if (n != SYS_write && n != SYS_writev) {
		dprintf(1, "SYSCALL 0x%2lx args: 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n", n, a, b, c, d, e, f);
	}
}
#endif // DUMP_SYSCALLS

long syscall(long n, ...)
{
	va_list ap;
	syscall_arg_t a,b,c,d,e,f;
	va_start(ap, n);
	a=va_arg(ap, syscall_arg_t);
	b=va_arg(ap, syscall_arg_t);
	c=va_arg(ap, syscall_arg_t);
	d=va_arg(ap, syscall_arg_t);
	e=va_arg(ap, syscall_arg_t);
	f=va_arg(ap, syscall_arg_t);
	va_end(ap);

#ifndef GRAALOS
	/*
	 * Map GraalOS-specific call to normal syscalls:
	 */
	if (n == VISORCALL_mmap_fixed_offset) {
		n = SYS_mmap;
	}
	if (n < 0) {
		dprintf(2, "unsupported VISOR syscall: 0x%x\n", n);
		exit(-1);
	}
#endif // GRAALOS

	return __syscall_ret(__syscall(n,a,b,c,d,e,f));
}

/**
* This function provides a semantically equivalent replacement for the syscall instruction on Linux. It expects the kernel syscall interface, using %rax for the syscall number, and %rdi, %rsi, %rdx, %r10, %r8 and %r9 for the arguments. The result will be stored in %rax directly.
*
* The main user of this function is Golang, which expects to be able to make direct syscalls.
 */
long __attribute__((naked)) syscall_direct() {
	// on GraalOS: %r10-13 are clobbered (as defined in SYSCALL_CLOBBER_COMMON)
	// on Linux: %rcx and %r11 are clobbered
	// Thus, we need to save %r10, %r12, and %r13
	__asm__ __volatile__(
		"pushq %%r10\n\t" \
		"pushq %%r12\n\t" \
		"pushq %%r13\n\t" \
		// this is required since the kernel interface uses %r10, and the C ABI uses %rcx for the 4th parameter
		"mov %%r10, %%rcx\n\t" \
		SYSCALL_ASM_SEQ \
		"popq %%r13\n\t" \
		"popq %%r12\n\t" \
		"popq %%r10\n\t" \
		"ret\n\t"::
	);
}
