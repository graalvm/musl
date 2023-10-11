.section .init
.extern __x86_return_thunk
#ifdef __CET__
	endbr64
#endif
	pop %rax
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif

.section .fini
#ifdef __CET__
	endbr64
#endif
	pop %rax
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
