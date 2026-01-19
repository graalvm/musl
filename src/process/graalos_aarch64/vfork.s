#include "syscall_internal.h"

.global vfork
.type vfork,%function
vfork:
	// make vfork fallthrough into __graalos_fork by setting the first arg to: SIGCHLD | CLONE_VM | CLONE_VFORK
	mov x0, 0x4111 // SIGCHLD | CLONE_VM | CLONE_VFORK

//	mov x8, 220    // SYS_clone
//	mov x0, 0x4111 // SIGCHLD | CLONE_VM | CLONE_VFORK
//	mov x1, 0
//	svc 0
//	.hidden __syscall_ret
//	b __syscall_ret




// int __graalos_fork(uint64_t flags %rdi)
.global __graalos_fork
.type __graalos_fork,@function

__graalos_fork:
	// Save the registers that the visorcall sequence clobbers in unused call args
	// r12 -> the saved return address -> x19
	// save x30 (lr) for ret and x29 (fp) for previous rets
	// unused arg registers (there are more if needed): x3, x4, x5
	mov x5, x19
	mov x4, x30
	mov x3, x29

	// Save registers that get clobbered
	stp x29, x30, [sp, #-16]!
	stp x27, x28, [sp, #-16]!
	stp x25, x26, [sp, #-16]!
	stp x23, x24, [sp, #-16]!
	stp x21, x22, [sp, #-16]!
	stp x19, x20, [sp, #-16]!

	/*
	 * Prepare a clone_params_t on stack. (pad by 8 bytes to get 16 byte align)
	 */
	sub x20, sp, #0x40 // x20 = sp - SIZEOF_CLONE_PARAMS
	adr x19, __graalos_fork_child_return // x19 = &__graalos_fork_child_return
	str x19, [x20, #0x0] // *(x20 + OFFS_FUNC) = x19
	str x0, [x20, #0x8] //  *(x20 + OFFS_FLAGS) = x0
	mov x21, sp
	str x21, [x20, #0x18] // *(x20 + OFFS_STACK) = sp (needs a gpr)

	// Call clone visorcall.
	mov x0, x20	// get params pointer
	mov x8, #__VISORCALL_clone // x8 syscall number (caller saved)
	adr x19, __graalos_fork_parent_return // ret for parent
	b __visorcall

__graalos_fork_child_return:
	ldp x19, x20, [sp], #16
	ldp x21, x22, [sp], #16
	ldp x23, x24, [sp], #16
	ldp x25, x26, [sp], #16
	ldp x27, x28, [sp], #16
	ldp x29, x30, [sp], #16

	// x0 must be zero for child (ret val)
	mov x0, xzr

	// return, no need for __syscall_ret
	ret

__graalos_fork_parent_return:
	// check for signals
	cmp x0, #-ESIGNAL
	beq __sig_handle

	// Do not rely on the values on stack. Throw them away
	add sp, sp, #0x60

	// restore from registers
	mov x19, x5
	mov x30, x4
	mov x29, x3

	// Tail-call __syscall_ret (x0 is result and first arg, no need to move)
	b __syscall_ret