.global __x86_return_thunk
.type   __x86_return_thunk,@function
__x86_return_thunk:

#ifdef __CET__
	endbr64
	pop     %r11
	push    %rcx
	mov     (%r11),%ecx
	add     $0x05e1f00d,%ecx
	jne     trap
	lfence
	pop     %rcx
	jmpq    *%r11
trap: 
#endif
// make this trap immediately without __CET__
	int3
