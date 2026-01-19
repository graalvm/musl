//.global __unmapself
//.type   __unmapself,%function
//__unmapself:
//	mov x8,#215 // SYS_munmap
//	svc 0
//	mov x8,#93 // SYS_exit
//	svc 0

#include "pthread_impl.h"
#include "syscall_internal.h"

_Noreturn void __unmapself(void *base, size_t size)
{
	// use __syscall here to avoid direct syscalls via svc
    __syscall2(SYS_munmap, base, size);
    __syscall1(SYS_exit, 0);
    __asm__ __volatile__("brk #0x0");

    __builtin_unreachable();
}