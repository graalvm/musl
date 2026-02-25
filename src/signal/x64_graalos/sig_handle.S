// Copyright (c) 2025 Oracle and/or its affiliates. All rights reserved.

#include "syscall_internal.h"

.text

.extern __sig_handle_inner
.hidden __sig_handle_inner

.global __sig_handle
.hidden __sig_handle
__sig_handle:
	// skip red zone and align stack
	mov %rsp, %r11
	sub $0x80, %rsp
	and $-0x10, %rsp
	push %r11

	// save registers
	push %r9
	push %r8
	push %rcx
	push %rdx
	push %rsi
	push %rdi
	push %r12

	// pass a pointer to the saved %r12 (i.e. saved %rip) as the first argument
	mov %rsp, %rdi

	// __sig_handle_inner returns {%rax = desired_rax, %rdx = sigmask}
	call __sig_handle_inner
	endbr64

	// did __sig_handle_inner return {*, -1}?
	cmp $-1, %rdx
	je done

	// call sigreturn
	mov %rax, %rdi // desired rax => first sigreturn argument
	mov %rdx, %rsi // sigmask => second sigreturn argument
	mov $0x8000000000000005, %rax // __VISORCALL_sigreturn
	lea sigreturn_return_target(%rip), %r12
	jmp __visorcall
sigreturn_return_target:
	endbr64

done:
	// restore registers
	pop %r12
	pop %rdi
	pop %rsi
	pop %rdx
	pop %rcx
	pop %r8
	pop %r9

	pop %rsp

	cmp $-ESIGNAL, %rax
	je __sig_handle

#ifdef __SANDBOX_SWCFI__
	mov (%r12), %r10d
	add $0x05e1f00d, %r10d
	jnz trap
#endif
	jmp *%r12

trap:
	int3

.size __sig_handle,.-__sig_handle
