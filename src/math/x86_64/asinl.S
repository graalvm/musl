.global asinl
.type asinl,@function
asinl:
	endbr64
	fldt 8(%rsp)
1:	fld %st(0)
	fld1
	fsub %st(0),%st(1)
	fadd %st(2)
	fmulp
	fsqrt
	fpatan
.extern __x86_return_thunk
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
