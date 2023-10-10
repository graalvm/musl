.section .init
.global _init
_init:
#ifdef __CET__
        endbr64
#endif
	push %rax

.section .fini
.global _fini
_fini:
#ifdef __CET__
        endbr64
#endif
	push %rax
