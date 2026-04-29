#include <sys/random.h>
#include "syscall.h"

ssize_t getrandom(void *buf, size_t buflen, unsigned flags)
{
#ifndef GRAALOS
	/* Host-mode musl must tolerate the private init flag when running outside graalhost. */
	flags &= ~GRND_INIT;
#endif
	return syscall_cp(SYS_getrandom, buf, buflen, flags);
}
