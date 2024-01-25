.text
.global __clone
.hidden __clone
.type   __clone,@function
.extern __x86_return_thunk
__clone:
	endbr64
	xor %eax,%eax
	mov $56,%al
	mov %rdi,%r11
	mov %rdx,%rdi
	mov %r8,%rdx
	mov %r9,%r8
	mov 8(%rsp),%r10
	mov %r11,%r9
	and $-16,%rsi
	sub $8,%rsi
	mov %rcx,(%rsi)
	syscall
	test %eax,%eax
	jnz 1f
	xor %ebp,%ebp
	pop %rdi
#ifdef __CET__
	mov (%r9),%ebx
    add $0x05e1f00d,%ebx
    je success
    int3
success:
    lfence
#endif
	call *%r9
	endbr64
	mov %eax,%edi
	xor %eax,%eax
	mov $60,%al
	syscall
	hlt
1:
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
