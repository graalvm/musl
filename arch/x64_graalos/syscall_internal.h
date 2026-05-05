// Copyright (c) 2023 Oracle and/or its affiliates. All rights reserved.

#define __VISORCALL_clone ((1ul << 63) | (1ul))
#define __VISORCALL_notify ((1ul << 63) | (3ul))
#define __VISORCALL_sigcall ((1ul << 63) | (4ul))
#define __VISORCALL_sigreturn ((1ul << 63) | (5ul))
#define __VISORCALL_clock_gettime ((1ul << 63) | 6ul)
#define __VISORCALL_fd_create ((1ul << 63) | (7ul))
// Reserve error values from 0 to 0xfff
#define __VISORCALL_fd_xcall5 ((1ul << 63) | (8ul))
#define __VISORCALL_fd_xcall6 ((1ul << 63) | (9ul))
#define __VISORCALL_fd_xread ((1ul << 63) | (10ul))
#define __VISORCALL_fd_xwrite ((1ul << 63) | (11ul))
#define __VISORCALL_fd_xverify ((1ul << 63) | (12ul))
#define __VISORCALL_xcall_wait_return   ((1ul << 63) | (13ul))
#define __VISORCALL_xcall6_setup         ((1ul << 63) | (14ul))

#define __VISORCALL_profiler_scope ((1ul << 63) | (17ul))

// The following constants are the valid arg0 op values
// for the __VISORCALL_profiler_scope visorcall.
#define __VISORCALL_profiler_scope_op_push 0ul
#define __VISORCALL_profiler_scope_op_pop 1ul

// Special return/error values
#define __XCALL_WAIT_EXIT_SUCCESS 0xffe

// __VISORCALL_xcall_wait_return flags
#define __XCALL_TO_WAIT 0x1
#define __XCALL_TO_RET  0x2

// Error values
#define ERESTARTSYS 512
#define ESIGNAL 513

// GraalOS descriptor types
#define VISOR_FDTYPE_CROSSISO 1             // cross-isolate access descriptor

// GraalOS descriptor create flags
#define GFD_CLOEXEC     0x80000
#define GFD_NONBLOCK    0x800

// XCall-specific GraalOS descriptor flags
#define GFD_AVX512_HI16_CALL 0x10000
#define GFD_AVX512_HI16_RET  0x20000
