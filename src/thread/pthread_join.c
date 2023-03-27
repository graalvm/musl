#define _GNU_SOURCE
#include "pthread_impl.h"
#include "stdio_impl.h"
#include "libc.h"
#include "lock.h"
#include <sys/mman.h>
#include <string.h>
#include <stddef.h>

static int is_thread_dead(pid_t pid, pid_t tid)
{
    long err = __syscall(SYS_tgkill, pid, tid, 0);
    return ((err < 0) && (err != -EAGAIN));
}

static void dummy1(pthread_t t)
{
}
weak_alias(dummy1, __tl_sync);

static int __pthread_timedjoin_np(pthread_t t, void **res, const struct timespec *at)
{
	int state, cs, r = 0;
    pid_t pid = __syscall(SYS_getpid);
    pid_t tid = t->tid;
	__pthread_testcancel();
	__pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	if (cs == PTHREAD_CANCEL_ENABLE) __pthread_setcancelstate(cs, 0);
	while ((state = t->detach_state) && r != ETIMEDOUT && r != EINVAL) {
		if (state >= DT_DETACHED) a_crash();
		r = __timedwait_cp(&t->detach_state, state, CLOCK_REALTIME, at, 1);
	}
	__pthread_setcancelstate(cs, 0);
	if (r == ETIMEDOUT || r == EINVAL) return r;
	__tl_sync(t);
	if (res) *res = t->result;
	if (t->map_base)
    {
        while (tid && !is_thread_dead(pid, tid))
            ;
        __munmap(t->map_base, t->map_size);
    }
	return 0;
}

int __pthread_join(pthread_t t, void **res)
{
	return __pthread_timedjoin_np(t, res, 0);
}

static int __pthread_tryjoin_np(pthread_t t, void **res)
{
	return t->detach_state==DT_JOINABLE ? EBUSY : __pthread_join(t, res);
}

weak_alias(__pthread_tryjoin_np, pthread_tryjoin_np);
weak_alias(__pthread_timedjoin_np, pthread_timedjoin_np);
weak_alias(__pthread_join, pthread_join);
