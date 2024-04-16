// Copyright (c) 2024 Oracle and/or its affiliates.  All right reserved.
#include "atomic.h"
#include "ksigaction.h"
#include "libc.h"
#include "lock.h"
#include "pthread_impl.h"
#include "syscall.h"
#include <errno.h>
#include <signal.h>
#include <string.h>

extern int __sigaction(int sig, const struct sigaction* restrict sa, struct sigaction* restrict old);

static pthread_mutex_t signal_thread_mutex = PTHREAD_MUTEX_INITIALIZER;
static int             signal_thread_inited = 0;
static int             signal_thread_started = 0;
static sigset_t        signal_thread_waitset;

static void* signal_handling_func(void*) {
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL); // block everything so this thread will not be signalled

    while (1) {
        // each iteration retrieve the current waitset while under lock
        pthread_mutex_lock(&signal_thread_mutex);
        sigset_t waitset = signal_thread_waitset;
        pthread_mutex_unlock(&signal_thread_mutex);

        siginfo_t       info;
        struct timespec ts;
        ts.tv_sec = ts.tv_nsec = -1; // special timeout to tell visor to deliver any signal with a handler
        int sig = sigtimedwait(&waitset, &info, &ts);
        if (sig > 0) {
            struct sigaction old;
            if (sigaction(sig, NULL, &old) == 0) {
                if (old.sa_flags & SA_SIGINFO) {
                    // TODO:  This thread is an extra one created by SYS_sigaction to service isolate signal handlers upon signal arrival.
                    //        The problems with this approach are that (a) signal handlers will be called from an otherwise unknown (to the
                    //        isolate) thread, and (b) sending signals to a specific thread to be handled there won't work.
                    if (old.sa_sigaction) {
                        ucontext_t context;
                        memset(&context, 0, sizeof(context));
                        // getcontext(&context);         -- not implemented my MUSL
                        old.sa_sigaction(sig, &info, &context);
                    }
                } else {
                    if (old.sa_handler) {
                        old.sa_handler(sig);
                    }
                }
            }
        }
    }

    signal_thread_started = 0;
    return NULL;
}

static void graalos_update_signal_handler_thread(int sig, int installed) {
    pthread_mutex_lock(&signal_thread_mutex);

    if (!signal_thread_inited) {
        sigemptyset(&signal_thread_waitset);
        signal_thread_inited = 1;
    }

    if (installed) {
        sigaddset(&signal_thread_waitset, sig);

        if (!signal_thread_started) {
            signal_thread_started = 1;

            // ensure that signal handling thread wakes on SIGSYS
            sigaddset(&signal_thread_waitset, SIGSYS);

            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

            pthread_t thread;
            pthread_create(&thread, &attr, signal_handling_func, &signal_thread_waitset);
        }
    } else {
        sigdelset(&signal_thread_waitset, sig);
    }

    pthread_mutex_unlock(&signal_thread_mutex);

    if (signal_thread_started) {
        // wake signal handling thread to deal with changes to its waitset
        raise(SIGSYS);
    }
}

int sigaction(int sig, const struct sigaction* restrict sa, struct sigaction* restrict old) {
    // call core MUSL implementation
    int result = __sigaction(sig, sa, old);

    if (sa && !result && (sig > 0)) {
        graalos_update_signal_handler_thread(sig, (uintptr_t)sa->sa_handler > 1UL);
    }

    return result;
}
