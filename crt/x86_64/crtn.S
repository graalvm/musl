.section .init
.extern __x86_return_thunk
	endbr64
	pop %rax
#ifdef __SANDBOX_SWCFI__
	jmp __x86_return_thunk
#else
	ret
#endif

.section .fini
	endbr64
	pop %rax
#ifdef __SANDBOX_SWCFI__
	jmp __x86_return_thunk
#else
	ret
#endif

