.global logl
.type logl,@function
logl:
	endbr64
	fldln2
	fldt 8(%rsp)
	fyl2x
.extern __x86_return_thunk
#ifdef __SANDBOX_SWCFI__
	jmp __x86_return_thunk
#else
	ret
#endif
