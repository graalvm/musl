.global feclearexcept
.type feclearexcept,@function
.extern __x86_return_thunk
feclearexcept:
		// maintain exceptions in the sse mxcsr, clear x87 exceptions
	endbr64
	mov %edi,%ecx
	and $0x3f,%ecx
	fnstsw %ax
	test %eax,%ecx
	jz 1f
	fnclex
1:	stmxcsr -8(%rsp)
	and $0x3f,%eax
	or %eax,-8(%rsp)
	test %ecx,-8(%rsp)
	jz 1f
	not %ecx
	and %ecx,-8(%rsp)
	ldmxcsr -8(%rsp)
1:	xor %eax,%eax
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif

.global feraiseexcept
.type feraiseexcept,@function
feraiseexcept:

	endbr64
	and $0x3f,%edi
	stmxcsr -8(%rsp)
	or %edi,-8(%rsp)
	ldmxcsr -8(%rsp)
	xor %eax,%eax
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif

.global __fesetround
.hidden __fesetround
.type __fesetround,@function
__fesetround:
	endbr64
	push %rax
	xor %eax,%eax
	mov %edi,%ecx
	fnstcw (%rsp)
	andb $0xf3,1(%rsp)
	or %ch,1(%rsp)
	fldcw (%rsp)
	stmxcsr (%rsp)
	shl $3,%ch
	andb $0x9f,1(%rsp)
	or %ch,1(%rsp)
	ldmxcsr (%rsp)
	pop %rcx
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif

.global fegetround
.type fegetround,@function
fegetround:
	endbr64
	push %rax
	stmxcsr (%rsp)
	pop %rax
	shr $3,%eax
	and $0xc00,%eax
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif

.global fegetenv
.type fegetenv,@function
fegetenv:
	endbr64
	xor %eax,%eax
	fnstenv (%rdi)
	stmxcsr 28(%rdi)
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif

.global fesetenv
.type fesetenv,@function
fesetenv:
	endbr64
	xor %eax,%eax
	inc %rdi
	jz 1f
	fldenv -1(%rdi)
	ldmxcsr 27(%rdi)
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
1:	push %rax
	push %rax
	pushq $0xffff
	pushq $0x37f
	fldenv (%rsp)
	pushq $0x1f80
	ldmxcsr (%rsp)
	add $40,%rsp
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif

.global fetestexcept
.type fetestexcept,@function
fetestexcept:
	endbr64
	and $0x3f,%edi
	push %rax
	stmxcsr (%rsp)
	pop %rsi
	fnstsw %ax
	or %esi,%eax
	and %edi,%eax
#ifdef __CET__
	jmp __x86_return_thunk
#else
	ret
#endif
