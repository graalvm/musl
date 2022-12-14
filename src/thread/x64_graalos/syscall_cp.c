#include "syscall.h"
//extern hidden long __cancel();
//
//long __syscall_cp_asm(volatile void *cancel, syscall_arg_t a1,
//                      syscall_arg_t a2, syscall_arg_t a3, syscall_arg_t a4,
//                      syscall_arg_t a5, syscall_arg_t a6, syscall_arg_t a7) {
//l1: __asm__ ("__cp_begin:");
//l2: __asm__ ("__cp_end:");
//    return 0;
//
//l3: __asm__ ("__cp_cancel:");
//    return __cancel();
//}
