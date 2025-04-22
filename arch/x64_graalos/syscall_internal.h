// Copyright (c) 2023 Oracle and/or its affiliates. All rights reserved.

#define __VISORCALL_clone ((1ul << 63) | (1ul))
#define __VISORCALL_notify ((1ul << 63) | (3ul))
#define __VISORCALL_sigcall ((1ul << 63) | (4ul))
#define __VISORCALL_sigreturn ((1ul << 63) | (5ul))
#define __VISORCALL_clock_gettime ((1ul << 63) | 6ul)

#define ERESTARTSYS 512
#define ESIGNAL 513
