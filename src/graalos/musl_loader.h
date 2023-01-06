// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#ifndef __MUSL_LOADER_H
#define __MUSL_LOADER_H

#include <elf.h>

#ifndef AUX_CNT
#define AUX_CNT 38
#endif

/* We need this for the struct match */
#define _AUX_CNT 38

struct auxv_entry {
    size_t key;
    size_t value;
};


struct _dso {
    unsigned char *base;
    char *name;
    size_t *dynv;
    struct dso *next;
    struct dso *prev;
    void *empty;
    unsigned char *map;
    size_t map_len;
    size_t relro_start, relro_end;
    Elf64_Phdr *phdr;
    int phnum;
    size_t phentsize;
    Elf64_Sym *syms;
    uint32_t *ghashtab;
    int16_t *versym;
    char *strings;
    size_t *got;
    struct tls_module tls;
};

struct musl_loader {
    int loader_fd;
    int library_fd;

    struct _dso loader_dso;
    struct _dso library_dso;

    struct auxv_entry auxv[_AUX_CNT];

    size_t last_aux_entry;

    void (*debug_state)();
    void *debug;

    void *stack;
    size_t stack_size;

    uintptr_t last_addr;

    size_t tls_size;
    size_t tls_align;
    size_t tls_cnt;

    int argc;
    char **argv;
    char **env;
    int (*entry)(int argc, char *argv[]);
    int (*clone_func)(int (*fn)(void *), void *child_stack, void *ml, int* tid, void* tls, volatile void* tl_lock, void* tp);
    long (*syscall_handler)(long n, long a1, long a2, long a3, long a4, long a5, long a6);

    int initial_tid;
};

#endif /* __MUSL_LOADER_H */
