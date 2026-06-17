// Copyright (c) 2025 Oracle and/or its affiliates. All rights reserved.

#include "syscall_internal.h"

// __sig_handle is called from inline assembly, so the compiler does not know about the call and does not move rsp in the caller.
// However, it still avoids using the stack beyond the red zone, so we can safely use any bytes above it, i.e. at lower addresses.
#define RED_ZONE 0x80

.cfi_sections .debug_frame

.text

.extern __sig_handle_inner
.hidden __sig_handle_inner

.global __sig_handle
.hidden __sig_handle
__sig_handle:
	.cfi_startproc simple

	// skip red zone
	sub $RED_ZONE, %rsp

	// save registers
	push %r12
	push %rbp
	mov %rsp, %rbp // rbp points to old rbp
	push %r9
	push %r8
	push %rcx
	push %rdx
	push %rsi
	push %rdi

	// align the stack pointer
	and $-0x10, %rsp

	// cfa = canonical frame address, the old %rsp value and at the same time the pointer
	// relative to which saved registers are resolved. Here reconstructed as a computation using the %rbp value:
	//       %rbp  ==  old %rsp - RED_ZONE - sizeof(pushed %r12) - sizeof(pushed %rbp)
	//   old %rsp  ==  %rbp + 0x10 + RED_ZONE
	.cfi_def_cfa %rbp, 0x10+RED_ZONE

	// The old (saved) register values, here reconstructed as loads from stack, at constant offsets from CFA.
	.cfi_offset %rip, -0x08-RED_ZONE
	.cfi_offset %rbp, -0x10-RED_ZONE
	.cfi_offset %r9,  -0x18-RED_ZONE
	.cfi_offset %r8,  -0x20-RED_ZONE
	.cfi_offset %rcx, -0x28-RED_ZONE
	.cfi_offset %rdx, -0x30-RED_ZONE
	.cfi_offset %rsi, -0x38-RED_ZONE
	.cfi_offset %rdi, -0x40-RED_ZONE

	// pass a pointer to the saved %r12 (i.e. saved %rip) as the first argument
	lea 8(%rbp), %rdi

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
	// pop stack alignment padding (if any)
	lea -48(%rbp), %rsp
	// restore registers
	pop %rdi
	pop %rsi
	pop %rdx
	pop %rcx
	pop %r8
	pop %r9
	pop %rbp
	pop %r12

	// pop red zone
	add $RED_ZONE, %rsp

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

	.cfi_endproc

.size __sig_handle,.-__sig_handle
