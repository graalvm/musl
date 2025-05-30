// Copyright (c) 2025 Oracle and/or its affiliates. All rights reserved.

#include "syscall_internal.h"
#include "vfork_internal.h"

.extern __x86_return_thunk
.extern __visorcall
.extern __vfork_clone_flags // defined in vfork_internal.c

/*
 * Functions in this file:
 * - int vfork(void) - the standard UNIX-like interface
 * - int __graalos_fork(uint64_t flags) - same as vfork (or a naive fork), except that it takes clone flags as an argument
 *
 * GraalOS visor, unlike Linux, calls a function in the child on clone. The function takes a single argument, passed in the clone_params_t.
 * Another, more low-level, way to look at this fact is: in the child, the syscall clobbers all registers, and sets %rsp, %rip, %rdi.
 * The purpose of the following two functions is to hide this difference and provide a Linux-like API to the application.
 *
 *
 * # Implementation
 *
 * Returning in the child, as on Linux, is achieved by:
 * - passing the current stack (%rsp) as the desired stack for the child
 * - passing a function that restores the register state and returns immediately as the desired child function
 *
 * One thing to keep in mind is that with vfork-like clones (CLONE_VFORK|CLONE_VM|...), the child runs first and overwrites the stack.
 * The return address (initially stored at %rsp) and everything above it (i.e. towards lower addresses) is lost before the parent runs.
 * Any vfork/__graalos_fork wrapper (whether in the application or in musl) must satisfy at least one of these conditions:
 * - it doesn't return in the parent
 * - it tail-calls (jumps to) vfork/__graalos_fork instead of calling it normally (since vfork/__graalos_fork is able to return safely)
 * - it loads the return address to a register eagerly, prior to the vfork/__graalos_fork call (mimicking vfork/__graalos_fork implementation)
 *
 * These conditions preclude splitting these two functions into a pure C wrapper/caller that prepares the clone_params_t and an asm callee,
 * since a caller in pure C can't satisfy any of the above conditions:
 * - not returning in the parent obviously goes against the specification
 * - tail-call is impossible to force with Clang unless the argument types of the callee match (so the caller can't pass the clone_params_t*),
 *   and it will not be done by default (and is incorrect to do) if there's a local clone_params_t on the stack requiring cleanup after the call
 * - loading/storing the return address in C would mean depending heavily on the compiler's use of the stack pointer
 *
 * Another way to prepare the clone_params_t in C is calling a C function from the __graalos_fork asm function.
 * The asm would still have to stack-allocate the clone_params_t and pass pointer to it and the stack pointer to the function,
 * replacing 5 instructions (2x lea to get the pointers, 3x member store) with another 5 (sub %rsp, 2x lea to get the pointers, call, endbr)
 * and creating additional constraints on the ordering of the instructions.
 */


// int vfork(void)
.global vfork
.type vfork,@function

vfork:
	endbr64
	mov __vfork_clone_flags(%rip), %rdi
	// jmp __graalos_fork - fallthrough


// int __graalos_fork(uint64_t flags %rdi)
.global __graalos_fork
.type __graalos_fork,@function

__graalos_fork:
	endbr64

	/*
	 * As per the calling convention, we must preserve:  %rbx, %rsp, %rbp, %r12, %r13, %r14, %r15.
	 * However, the visorcall sequence will clobber %r12 and %r13.
	 * We can save them into the last 2 visorcall arg registers, since those registers are preserved and unused.
	 * Saving them to the stack is not sufficient, since the child would overwrite them before the parent manages to pop them.
	 */
	mov %r12, %r8
	mov %r13, %r9

	/*
	 * The return address on the stack may also be rewritten by the child. Save it to another unused visorcall arg register.
	 */
	mov (%rsp), %rcx

	/*
	 * All registers except %rsp will be clobbered in the child. Fortunately, the child can normally use the stack because:
	 * - in case of fork (flags=0), both the child and the parent have their own copies of the address space
	 * - in case of vfork (flags=CLONE_VM|CLONE_VFORK), the child gets to read the stack before the parent can overwrite it
	 * Save all the registers we must preserve (except %rsp) to the stack.
	 */
	push %r15
	push %r14
	push %r13
	push %r12
	push %rbp
	push %rbx

	/*
	 * Prepare a clone_params_t on stack. Pointer in %rax because we still need %rdi.
	 */
	lea -SIZEOF_CLONE_PARAMS(%rsp), %rax
	lea __graalos_fork_child_return(%rip), %r12 /* tmp */
	mov %r12, OFFSETOF_CLONE_PARAMS_FUNC(%rax)
	mov %rdi, OFFSETOF_CLONE_PARAMS_FLAGS(%rax)
	mov %rsp, OFFSETOF_CLONE_PARAMS_STACK(%rax)

	/*
	 * Call clone visorcall.
	 */
	mov %rax, %rdi
	mov $__VISORCALL_clone, %rax
	lea __graalos_fork_parent_return(%rip), %r12
	jmp __visorcall

__graalos_fork_child_return:
	endbr64

	/* All registers except %rsp clobbered. Restore the ones we have to preserve. */
	pop %rbx
	pop %rbp
	pop %r12
	pop %r13
	pop %r14
	pop %r15

	/* In child, fork/vfork should return 0. (Currently, %rax contains garbage.) */
	xor %eax, %eax

	/* No need for __syscall_ret here. If we are in child, we know clone succeeded. */
	jmp __x86_return_thunk

__graalos_fork_parent_return:
	endbr64

	/* Handle signals, if any. */
	cmp $-ESIGNAL, %rax
	je __sig_handle

	/* Do not rely on the values on stack. Throw them away (including the return address). */
	add $0x38, %rsp

	/* Restore from registers instead. */
	mov %r9, %r13
	mov %r8, %r12

	/* Push the reliable return address to stack. */
	push %rcx

	/* Tail-call __syscall_ret. */
	mov %rax, %rdi
	jmp __syscall_ret
