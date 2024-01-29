.global atanl
.type atanl,@function
atanl:
	endbr64
	fldt 8(%rsp)
	fld1
	fpatan
.extern __x86_return_thunk
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
