.section .init
.global _init
_init:
#ifdef UNTRUSTED
        endbr64
#endif
	push %rax

.section .fini
.global _fini
_fini:
#ifdef UNTRUSTED
        endbr64
#endif
	push %rax
