.global log10l
.type log10l,@function
log10l:
#ifdef __CET__
        endbr64
#endif
	fldlg2
	fldt 8(%rsp)
	fyl2x
	ret
