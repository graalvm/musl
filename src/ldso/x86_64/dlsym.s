.text
.global dlsym
.hidden __dlsym
.type dlsym,@function
dlsym:
#ifdef __CET__
	endbr64
#endif
	mov (%rsp),%rdx
	jmp __dlsym
