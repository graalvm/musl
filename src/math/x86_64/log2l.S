.global log2l
.type log2l,@function
log2l:
	endbr64
	fld1
	fldt 8(%rsp)
	fyl2x
.extern __x86_return_thunk
#ifdef __SANDBOX_SWCFI__
	jmp __x86_return_thunk
#else
	ret
#endif
