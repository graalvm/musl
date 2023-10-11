.global vfork
.type vfork,@function
vfork:
#ifdef __CET__
	endbr64
#endif
	pop %rdx
	mov $58,%eax
	syscall
	push %rdx
	mov %rax,%rdi
	.hidden __syscall_ret
	jmp __syscall_ret
