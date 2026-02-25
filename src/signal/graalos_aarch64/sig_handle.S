// Copyright (c) 2025 Oracle and/or its affiliates. All rights reserved.

#include "syscall_internal.h"

.text

.extern __sig_handle_inner
.hidden __sig_handle_inner

.global __sig_handle
.hidden __sig_handle
__sig_handle:
	// AFAIK there is no stack red zone in the AAPCS64 ABI, unlike x86_64
	// x19 is the equivalent of x12 here
	// x22 holds the old arg0
	// save: x19-x28, x29 (fp), x30 (lr)

	// save rsp in x11
	mov x11, sp

	// save on stack
	stp x7, x11, [sp, #-16]!
	stp x5, x6, [sp, #-16]!
	stp x3, x4, [sp, #-16]!
	stp x1, x2, [sp, #-16]!
	stp x30, x0, [sp, #-16]!
	stp x28, x29, [sp, #-16]!
	stp x26, x27, [sp, #-16]!
	stp x24, x25, [sp, #-16]!
	stp x22, x23, [sp, #-16]!
	stp x20, x21, [sp, #-16]!
	stp x8, x19, [sp, #-16]!


	// __sig_handle_inner is tricky on aarch64, because the x0 register
	// is shared for the first call arg and the return value.
	// there are two possible scenarios, namely:
	// (1) we have to re-execute a system call
	//	- in this case, x0 needs to hold the original call argument, which is already gone from x0
	//	- and additionally, x8 needs to hold the syscall number
	//	--> on x86, rax would hold the syscall number, and rdi the call arg (saved and restored)
	//	--> but on aarch64, the jump to __visorcall already overrides x0 as call arg
	// (2) we have to return the result of a call
	//	- in this case, x0 needs to be the return value
	// 
	// in order to restore the first call arg, we save the original x0 in x22
	// before jumping to __visorcall.
	// we then pass this arg to __sig_handle_inner
	// along with a pointer to the saved x8 (to-be-restored)
	// if __sig_handle_inner indicates a restart is required,
	// saved x8 is updated to the syscall number
	// and x0 is restored to the call arg
	// otherwise, saved x8 is unchanged and x0 is the return value


	// first arg is pointer to saved x19 (r12), aka sp+8
	// second arg is pointer to saved x8, aka sp
	// third arg is the original call's first arg (old x0)
	add x0, sp, #8	// x0 = sp + 8
	mov x1, sp	// x1 = sp
	mov x2, x22 // x2 = x22
	bl __sig_handle_inner

	// did __sig_handle_inner return {*, -1}?
	cmp x1, #-1
	beq done

	// call sigreturn. x0=desired_rax (result), x1=sigmask
	movz x8, #0x8000, lsl #48 // x8 = 0x8000_0000_0000_0000
	movk x8, #0x5 // x8 = 0x8000_0000_0000_0005  __VISORCALL_sigreturn ((1ul << 63) | (5ul))

	adr x19, sigreturn_return_target
    b __visorcall
	
sigreturn_return_target:

done:
	// restore registers
	ldp x8, x19, [sp], #16		// pop into x8 (syscall no, if needed), and x19
	ldp x20, x21, [sp], #16		// pop x20 and x21 (callee saved)
	ldp x22, x23, [sp], #16		// pop x22 and x23 (callee saved)
	ldp x24, x25, [sp], #16		// pop x24 and x25 (callee saved)
	ldp x26, x27, [sp], #16		// pop x26 and x27 (callee saved)
	ldp x28, x29, [sp], #16		// pop x28 (callee saved) and x29 (fp)
	ldp x30, x1, [sp], #16		// pop into temp x1 to not desired_rax, pop x30 (lr, ret), 
	ldp x1, x2, [sp], #16		// pop x1 and x2 (arg1, arg2)
	ldp x3, x4, [sp], #16		// pop x3 and x4 (arg3, arg4)
	ldp x5, x6, [sp], #16		// pop x5 and x6 (arg5, arg6)
	ldp x7, x11, [sp], #16		// pop x7 (arg7) and x10 (temp caller-saved reg for sp)

	mov sp, x11

	cmp x0, #-ESIGNAL
	beq __sig_handle

	br x19

.size __sig_handle,.-__sig_handle
