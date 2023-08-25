.global atanl
.type atanl,@function
atanl:
#ifdef __CET__
        endbr64
#endif
	fldt 8(%rsp)
	fld1
	fpatan
	ret
