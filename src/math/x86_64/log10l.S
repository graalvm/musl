.global log10l
.type log10l,@function
log10l:
	endbr64
	fldlg2
	fldt 8(%rsp)
	fyl2x
.extern __x86_return_thunk
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
