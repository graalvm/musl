// Copyright (c) 2025 Oracle and/or its affiliates. All rights reserved.
#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include "syscall.h"
#include "syscall_internal.h"

int __clock_gettime(clockid_t clk, struct timespec *ts)
{
	long r = __syscall1(__VISORCALL_clock_gettime, clk);    // modified GraalOS clock_gettime returns time in 63-bit return value
    if (r > 0) {
        // decode the return value and put result into the timespec
        long nsec = (r & 0xffffful) << 10;
        long sec = r >> (20);
        ts->tv_sec = sec;
        ts->tv_nsec = nsec;
        return 0;
    }
	return __syscall_ret(r);
}

weak_alias(__clock_gettime, clock_gettime);
