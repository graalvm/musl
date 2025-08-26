// Copyright (c) 2023 Oracle and/or its affiliates. All rights reserved.

#define __VISORCALL_clone ((1ul << 63) | (1ul))
#define __VISORCALL_notify ((1ul << 63) | (3ul))
#define __VISORCALL_sigcall ((1ul << 63) | (4ul))
#define __VISORCALL_sigreturn ((1ul << 63) | (5ul))
#define __VISORCALL_clock_gettime ((1ul << 63) | 6ul)
#define __VISORCALL_fd_create ((1ul << 63) | (7ul))
#define __VISORCALL_fd_xcall ((1ul << 63) | (8ul))
#define __VISORCALL_fd_xread ((1ul << 63) | (9ul))
#define __VISORCALL_fd_xwrite ((1ul << 63) | (10ul))
#define __VISORCALL_fd_xverify ((1ul << 63) | (11ul))

// Error values
#define ERESTARTSYS 512
#define ESIGNAL 513

// GraalOS descriptor types
#define VISOR_FDTYPE_CROSSISO 1             // cross-isolate access descriptor

// GraalOS descriptor create flags
#define GFD_CLOEXEC     0x80000
#define GFD_NONBLOCK    0x800
