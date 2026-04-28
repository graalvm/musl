#include <elf.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>
#include "pthread_impl.h"
#include "syscall.h"
#include "atomic.h"
#include "libc.h"

static void dummy(void) {}
weak_alias(dummy, _init);

extern weak hidden void (*const __init_array_start)(void), (*const __init_array_end)(void);

static void dummy1(void *p) {}
weak_alias(dummy1, __init_ssp);

#define AUX_CNT 38

#ifdef GRAALOS
static char **main_argv;

/* Seed the stack bounds here from the virtual stack limit and initial stack data so
 * pthread_getattr_np can use the normal fast path instead of the Linux-specific
 * probing fallback. */
static void init_main_thread_stack(char **envp)
{
	struct rlimit stack_limit;
	char **p;
	char *highest_string = 0;
	uintptr_t stack_top;
	pthread_t self;

	if (__syscall(SYS_prlimit64, 0, RLIMIT_STACK, 0, &stack_limit) < 0) return;
	if (stack_limit.rlim_cur == SYSCALL_RLIM_INFINITY || !stack_limit.rlim_cur) return;

	for (p = main_argv; p && *p; p++) {
		if ((uintptr_t)*p > (uintptr_t)highest_string)
			highest_string = *p;
	}
	for (p = envp; p && *p; p++) {
		if ((uintptr_t)*p > (uintptr_t)highest_string)
			highest_string = *p;
	}

	stack_top = (uintptr_t)libc.auxv;
	if ((uintptr_t)highest_string > stack_top)
		stack_top = (uintptr_t)highest_string + strlen(highest_string) + 1;
	stack_top += -(uintptr_t)stack_top & (libc.page_size-1);

	self = __pthread_self();
	self->stack = (void *)stack_top;
	self->stack_size = stack_limit.rlim_cur;
}
#endif

#ifdef __GNUC__
__attribute__((__noinline__))
#endif
void __init_libc(char **envp, char *pn)
{
	size_t i, *auxv, aux[AUX_CNT] = { 0 };
	__environ = envp;
	for (i=0; envp[i]; i++);
	libc.auxv = auxv = (void *)(envp+i+1);
	for (i=0; auxv[i]; i+=2) if (auxv[i]<AUX_CNT) aux[auxv[i]] = auxv[i+1];
	__hwcap = aux[AT_HWCAP];
	if (aux[AT_SYSINFO]) __sysinfo = aux[AT_SYSINFO];
	libc.page_size = aux[AT_PAGESZ];

	if (!pn) pn = (void*)aux[AT_EXECFN];
	if (!pn) pn = "";
	__progname = __progname_full = pn;
	for (i=0; pn[i]; i++) if (pn[i]=='/') __progname = pn+i+1;

	__init_tls(aux);
#ifdef GRAALOS
	init_main_thread_stack(envp);
#endif
	__init_ssp((void *)aux[AT_RANDOM]);

	if (aux[AT_UID]==aux[AT_EUID] && aux[AT_GID]==aux[AT_EGID]
		&& !aux[AT_SECURE]) return;

	struct pollfd pfd[3] = { {.fd=0}, {.fd=1}, {.fd=2} };
	int r =
#ifdef SYS_poll
	__syscall(SYS_poll, pfd, 3, 0);
#else
	__syscall(SYS_ppoll, pfd, 3, &(struct timespec){0}, 0, _NSIG/8);
#endif
	if (r<0) a_crash();
	for (i=0; i<3; i++) if (pfd[i].revents&POLLNVAL)
		if (__sys_open("/dev/null", O_RDWR)<0)
			a_crash();
	libc.secure = 1;
}

static void libc_start_init(void)
{
	_init();
	uintptr_t a = (uintptr_t)&__init_array_start;
	for (; a<(uintptr_t)&__init_array_end; a+=sizeof(void(*)()))
		(*(void (**)(void))a)();
}

weak_alias(libc_start_init, __libc_start_init);

typedef int lsm2_fn(int (*)(int,char **,char **), int, char **);
static lsm2_fn libc_start_main_stage2;

int __libc_start_main(int (*main)(int,char **,char **), int argc, char **argv,
	void (*init_dummy)(), void(*fini_dummy)(), void(*ldso_dummy)())
{
	char **envp = argv+argc+1;

#ifdef GRAALOS
	main_argv = argv;
#endif

	/* External linkage, and explicit noinline attribute if available,
	 * are used to prevent the stack frame used during init from
	 * persisting for the entire process lifetime. */
	__init_libc(envp, argv[0]);

	/* Barrier against hoisting application code or anything using ssp
	 * or thread pointer prior to its initialization above. */
	lsm2_fn *stage2 = libc_start_main_stage2;
	__asm__ ( "" : "+r"(stage2) : : "memory" );
	return stage2(main, argc, argv);
}

static int libc_start_main_stage2(int (*main)(int,char **,char **), int argc, char **argv)
{
	char **envp = argv+argc+1;
	__libc_start_init();

	/* Pass control to the application */
	exit(main(argc, argv, envp));
	return 0;
}
