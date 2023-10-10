.global __x86_return_thunk

.section .init
	pop %rax
#ifdef __CET__
	jmp __x86_return_thunk
#else
        ret
#endif
.section .fini
	pop %rax
#ifdef __CET__
	jmp __x86_return_thunk
#else
        ret
#endif
