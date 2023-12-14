// Copyright (c) 2022-2023 Oracle and/or its affiliates. All rights reserved.
#ifndef GRAALOS_MUSL_LOADER_H
#define GRAALOS_MUSL_LOADER_H

#include <elf.h>
#include <stddef.h>

#include "musl_types.h"
#include "../../../../src/internal/pthread_impl.h"

#ifdef __cplusplus
namespace musl {
#endif // __cplusplus

#ifndef AUX_CNT
#define AUX_CNT 38
#endif

/* We need this for the struct match */
#define _AUX_CNT 38

#ifndef LIBC_H
struct tls_module {
    struct tls_module *next;
    void *image;
    size_t len, size, align, offset;
};
#endif

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

typedef int (*clone_fn_t)(thread_fn_t fn, void *child_stack,
                          void *ml, int *tid, void *tls,
                          volatile void *tl_lock, void *tp);

struct musl_loader {
    int loader_fd;
    int library_fd;

    struct _dso loader_dso;

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
    const char *entry_name;
    main_fn_t entry;
    clone_fn_t clone_func;
    syscall_handler_t syscall_handler;
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* GRAALOS_MUSL_LOADER_H */
