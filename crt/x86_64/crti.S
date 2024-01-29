.section .init
.global _init
_init:
	endbr64
	push %rax

.section .fini
.global _fini
_fini:
	endbr64
	push %rax
