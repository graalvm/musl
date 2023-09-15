.global log2l
.type log2l,@function
log2l:
#ifdef __CET__
	endbr64
#endif
	fld1
	fldt 8(%rsp)
	fyl2x
.extern __x86_return_thunk
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
