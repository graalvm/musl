.global atan2l
.type atan2l,@function
atan2l:
#ifdef __CET__
	endbr64
#endif
	fldt 8(%rsp)
	fldt 24(%rsp)
	fpatan
.extern __x86_return_thunk
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
