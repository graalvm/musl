// Copyright (c) 2025 Oracle and/or its affiliates. All rights reserved.

#include <errno.h>
#include <signal.h>
#include <stdint.h>

#include "include/graalos/musl_thread.h"
#include "syscall_arch.h"
#include "syscall_internal.h"

struct r {
	long desired_rax;
	uint64_t sigmask;
};

hidden struct r __sig_handle_inner(uint64_t *saved_rip_ptr) {
	struct sigcall_res sigcall_res;
	long status = __syscall1(__VISORCALL_sigcall, (long)&sigcall_res);

	long desired_rax = sigcall_res.syscall_result;

	switch (status) {
		case 0:
			// no signal to handle.
			// return immediately
			return (struct r) {desired_rax, -1};
		case 1:
			// calling a handler without SA_RESTART.
			// if the syscall returned -ERESTARTSYS, just translate it to -EINTR
			if (desired_rax == -ERESTARTSYS) {
				desired_rax = -EINTR;
			}
			break;
		case 2:
			// calling a handler with SA_RESTART.
			// if the syscall returned -ERESTARTSYS, roll back to the state before the syscall
			if (desired_rax == -ERESTARTSYS) {
				desired_rax = sigcall_res.syscall_number;
				*saved_rip_ptr -= 16;
			}
			break;
		default:
			__builtin_trap();
	}

	// call the handler
	((void (*)(int, siginfo_t *, void *))sigcall_res.handler)(sigcall_res.info.si_signo, &sigcall_res.info, NULL);

	return (struct r) {desired_rax, sigcall_res.sigmask};
}
