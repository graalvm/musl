
#define __SYSCALL_LL_E(x) (x)
#define __SYSCALL_LL_O(x) (x)

// this makes sure __syscall_cp_c doesnt call __syscall_cp_asm and instead goes through __syscallN,
// among other things
#define GRAALOS

#define __SYSCALL_STRINGIFY0(toks) #toks
#define __SYSCALL_STRINGIFY(toks) __SYSCALL_STRINGIFY0(toks)

#include "syscall_internal.h"

	
// load return address into x19
// save x0 (1st call arg) in x22
// because x0 will contain return value of the syscall
// but in case we branch to __sig_handle we need
// the original x0 to repeat the syscall
// x23 is used to lookup libc.visorcall
#define SYSCALL_ASM_SEQ                             \
    "adr x19, return_target%=\n\t"					\
	"mov x22, x0\n\t"								\
    "b __visorcall\n\t"                             \
    "return_target%=:\n\t"                          \
	"cmp x0, #-"__SYSCALL_STRINGIFY(ESIGNAL)"\n\t"	\
	"beq __sig_handle\n\t"   \

// GraalOS version:
// x19 return target
// x21 libc.visorcall
// x22 to save the first arg (x0) for __sig_handle
// x23 temp for libc.visorcall lookup
// in visorcall.S:
// x11 its
// x10 sp
// x9 prot sp
// x17 fpsr
// x16 fpcr
// x14 visor_dispatch
#define SYSCALL_CLOBBER_COMMON "x19", "x21", "x22", "x23", "cc", "memory", "x9", "x10", "x11", "x12", "x14", "x16", "x17"
#define __asm_syscall(...) do { \
__asm__ __volatile__ ( SYSCALL_ASM_SEQ \
: "=r"(x0) : __VA_ARGS__ : SYSCALL_CLOBBER_COMMON); \
return x0; \
} while (0)

// original aarch64 version:
// #define __asm_syscall(...) do { \
// 	__asm__ __volatile__ ( "svc 0" \
// 	: "=r"(x0) : __VA_ARGS__ : "memory", "cc"); \
// 	return x0; \
// 	} while (0)

static inline long __syscall0(long n)
{
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0");
	__asm_syscall("r"(x8));
}

static inline long __syscall1(long n, long a)
{
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	__asm_syscall("r"(x8), "0"(x0));
}

static inline long __syscall2(long n, long a, long b)
{
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	register long x1 __asm__("x1") = b;
	__asm_syscall("r"(x8), "0"(x0), "r"(x1));
}

static inline long __syscall3(long n, long a, long b, long c)
{
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	register long x1 __asm__("x1") = b;
	register long x2 __asm__("x2") = c;
	__asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2));
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	register long x1 __asm__("x1") = b;
	register long x2 __asm__("x2") = c;
	register long x3 __asm__("x3") = d;
	__asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3));
}

static inline long __syscall5(long n, long a, long b, long c, long d, long e)
{
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	register long x1 __asm__("x1") = b;
	register long x2 __asm__("x2") = c;
	register long x3 __asm__("x3") = d;
	register long x4 __asm__("x4") = e;
	__asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4));
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	register long x1 __asm__("x1") = b;
	register long x2 __asm__("x2") = c;
	register long x3 __asm__("x3") = d;
	register long x4 __asm__("x4") = e;
	register long x5 __asm__("x5") = f;
	__asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5));
}

#define VDSO_USEFUL
#define VDSO_CGT_SYM "__kernel_clock_gettime"
#define VDSO_CGT_VER "LINUX_2.6.39"

#define IPC_64 0
