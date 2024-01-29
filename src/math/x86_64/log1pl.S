.global log1pl
.type log1pl,@function
.extern __x86_return_thunk
log1pl:
	endbr64
	mov 14(%rsp),%eax
	fldln2
	and $0x7fffffff,%eax
	fldt 8(%rsp)
	cmp $0x3ffd9400,%eax
	ja 1f
	fyl2xp1
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
1:	fld1
	faddp
	fyl2x
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif

