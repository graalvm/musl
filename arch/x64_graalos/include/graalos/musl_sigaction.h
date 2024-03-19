// Copyright (c) 2024 Oracle and/or its affiliates.  All right reserved.
#ifdef __cplusplus
#ifndef GRAALOS
#define GRAALOS
#endif
namespace musl {
#endif

struct k_sigaction {
    void (*handler)(int);
    unsigned long flags;
    void (*restorer)(void);
    unsigned mask[2];
};

#ifdef __cplusplus
} // namespace musl
#endif