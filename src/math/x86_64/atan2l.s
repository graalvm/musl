.global atan2l
.type atan2l,@function
atan2l:
#ifdef __CET__
        endbr64
#endif
	fldt 8(%rsp)
	fldt 24(%rsp)
	fpatan
	ret
