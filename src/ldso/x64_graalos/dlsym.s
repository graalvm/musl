# Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
.text
.global dlsym
.hidden __dlsym
.type dlsym,@function
dlsym:
	mov (%rsp),%rdx
	jmp __dlsym
