// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#ifndef GRAALOS_MUSL_TYPES_H
#define GRAALOS_MUSL_TYPES_H

#ifdef __cplusplus
namespace musl {
#endif // __cplusplus

typedef int (*main_fn_t)(int argc, char *argv[]);

/// The user supplied thread start routine
typedef int (*thread_fn_t)(void *arg);

typedef long (*syscall_handler_t)(long n, long a1, long a2, long a3, long a4,
                                  long a5, long a6);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // GRAALOS_MUSL_TYPES_H
