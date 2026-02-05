//.global __restore
//.hidden __restore
//.type __restore,%function
//__restore:
//.global __restore_rt
//.hidden __restore_rt
//.type __restore_rt,%function
//__restore_rt:
//	mov x8,#139 // SYS_rt_sigreturn
//	svc 0
#include "pthread_impl.h"
#include "syscall_internal.h"

void __restore()
{
	// use __syscall here to avoid direct syscalls via svc
    __syscall0(SYS_rt_sigreturn);
}

void __restore_rt()
{
	// use __syscall here to avoid direct syscalls via svc
    __syscall0(SYS_rt_sigreturn);
}