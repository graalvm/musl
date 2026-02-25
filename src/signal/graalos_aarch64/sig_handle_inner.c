// Copyright (c) 2025 Oracle and/or its affiliates. All rights reserved.

#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stddef.h>

//#include "include/graalos/musl_thread.h"
#include "syscall_arch.h"
#include "syscall_internal.h"

struct r {
	long desired_rax;
	uint64_t sigmask;
};

struct sigcall_res {
    uint64_t  handler;
    uint64_t  syscall_number;
    uint64_t  syscall_result;
    uint64_t  sigmask;
    siginfo_t info;
};

hidden struct r __sig_handle_inner(uint64_t *saved_rip_ptr, uint64_t *saved_x8_ptr, uint64_t old_arg0) {
	struct sigcall_res sigcall_res;
	long status = __syscall1(__VISORCALL_sigcall, (long)&sigcall_res);

	long desired_x0 = sigcall_res.syscall_result;

	switch (status) {
		case 0:
			// no signal to handle.
			// return immediately
			return (struct r) {desired_x0, -1};
		case 1:
			// calling a handler without SA_RESTART.
			// if the syscall returned -ERESTARTSYS, just translate it to -EINTR
			if (desired_x0 == -ERESTARTSYS) {
				desired_x0 = -EINTR;
			}
			break;
		case 2:
			// calling a handler with SA_RESTART.
			// if the syscall returned -ERESTARTSYS, roll back to the state before the syscall
			if (desired_x0 == -ERESTARTSYS) {
				*saved_x8_ptr = sigcall_res.syscall_number;
				desired_x0 = old_arg0;
				*saved_rip_ptr -= 12; // on aarch64: -4 for b __visorcall, -4 for mov x22, x0, -4 for adr x19, return_target%=, thats a reset
			}
			break;
		default:
			__builtin_trap();
	}

	// call the handler
	((void (*)(int, siginfo_t *, void *))sigcall_res.handler)(sigcall_res.info.si_signo, &sigcall_res.info, NULL);

	return (struct r) {desired_x0, sigcall_res.sigmask};
}


