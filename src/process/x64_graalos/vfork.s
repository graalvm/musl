// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
.global vfork
.type vfork,@function
vfork:
#ifdef __CET__
	endbr64
#endif
	mov $-38,%eax
	mov %rax,%rdi
	.hidden __syscall_ret
	jmp __syscall_ret
