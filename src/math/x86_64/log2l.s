.global log2l
.type log2l,@function
log2l:
#ifdef __CET__
        endbr64
#endif
	fld1
	fldt 8(%rsp)
	fyl2x
	ret
