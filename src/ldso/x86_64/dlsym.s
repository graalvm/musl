.text
.global dlsym
.hidden __dlsym
.type dlsym,@function
dlsym:
	endbr64
	mov (%rsp),%rdx
	jmp __dlsym
