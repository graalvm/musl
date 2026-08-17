#include <stdlib.h>
#include "syscall.h"

static long __exit_syscall_maybe_restart(long n, long a)
{
	long ret = __syscall1(n, a);

#ifdef GRAALOS
	while (ret == -ERESTARTSYS)
		ret = __syscall1(n, a);
#endif
	return ret;
}

_Noreturn void _Exit(int ec)
{
	__exit_syscall_maybe_restart(SYS_exit_group, ec);
	for (;;) __syscall(SYS_exit, ec);
}
