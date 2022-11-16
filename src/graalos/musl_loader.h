#ifndef __MUSL_LOADER_H
#define __MUSL_LOADER_H

#include <elf.h>

#define AUX_CNT 32

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

    struct auxv_entry auxv[AUX_CNT];

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
    long (*thread_syscall)(long n, long a1, long a2, long a3, long a4, long a5, long a6);

    int initial_tid;
};

#endif /* __MUSL_LOADER_H */
