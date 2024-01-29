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
