.global logl
.type logl,@function
logl:
#ifdef __CET__
        endbr64
#endif
	fldln2
	fldt 8(%rsp)
	fyl2x
	ret
