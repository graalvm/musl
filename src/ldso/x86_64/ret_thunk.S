.global __x86_return_thunk
.type   __x86_return_thunk,@function
__x86_return_thunk:
        endbr64
#ifdef __CET__
        pop     %r11
        push    %rcx
        mov     (%r11),%ecx
        add     $0x05e1f00d,%ecx
        jne     trap
        pop     %rcx
        lfence
        jmpq    *%r11
trap: 
        int3
#else
        retq
#endif

