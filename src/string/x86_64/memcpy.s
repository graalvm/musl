.global memcpy
.global __memcpy_fwd
.hidden __memcpy_fwd
.type memcpy,@function
.extern __x86_return_thunk
memcpy:
__memcpy_fwd:
#ifdef __CET__
	endbr64
#endif
	mov %rdi,%rax
	cmp $8,%rdx
	jc 1f
	test $7,%edi
	jz 1f
2:	movsb
	dec %rdx
	test $7,%edi
	jnz 2b
1:	mov %rdx,%rcx
	shr $3,%rcx
	rep
	movsq
	and $7,%edx
	jz 1f
2:	movsb
	dec %edx
	jnz 2b
1:
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
