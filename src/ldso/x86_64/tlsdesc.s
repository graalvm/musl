.text
.global __tlsdesc_static
.hidden __tlsdesc_static
.extern __x86_return_thunk
.type __tlsdesc_static,@function
__tlsdesc_static:
#ifdef __CET__
	endbr64
#endif
	mov 8(%rax),%rax
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif

.global __tlsdesc_dynamic
.hidden __tlsdesc_dynamic
.type __tlsdesc_dynamic,@function
__tlsdesc_dynamic:
#ifdef __CET__
	endbr64
#endif
	mov 8(%rax),%rax
	push %rdx
	mov %fs:8,%rdx
	push %rcx
	mov (%rax),%rcx
	mov 8(%rax),%rax
	add (%rdx,%rcx,8),%rax
	pop %rcx
	sub %fs:0,%rax
	pop %rdx
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
