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

static volatile int signal_thread_started = 0;
static sigset_t     signal_thread_waitset;

static void* signal_handling_func(void* waitset) {
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL); // block everything so this thread will not be signalled

    while (1) {
        siginfo_t       info;
        struct timespec ts;
        ts.tv_sec = ts.tv_nsec = -1; // special timeout to tell visor to deliver any signal with a handler
        int sig = sigtimedwait((sigset_t*)waitset, &info, &ts);
        if (sig > 0) {
            fprintf(stderr, "signal_handling_func return %d\n", sig);

            struct sigaction old;
            if (sigaction(sig, NULL, &old) == 0) {
                if (old.sa_flags & SA_SIGINFO) {
                    // TODO:  This thread is an extra one created by SYS_sigaction to service isolate signal handlers upon signal arrival.
                    //        The problems with this approach are that (a) signal handlers will be called from an otherwise unknown (to the
                    //        isolate) thread, and (b) sending signals to a specific thread to be handled there won't work.
                    if (old.sa_sigaction) {
                        fprintf(stderr, "signal_handling_func calling action handler for %d\n", sig);
                        ucontext_t context;
                        // getcontext(&context);         -- not implemented my MUSL
                        memset(&context, 0, sizeof(context));
                        old.sa_sigaction(sig, &info, &context);
                    } else {
                        fprintf(stderr, "signal_handling_func did not find an action handler for %d\n", sig);
                    }
                } else {
                    if (old.sa_handler) {
                        fprintf(stderr, "signal_handling_func calling handler for %d\n", sig);
                        old.sa_handler(sig);
                    } else {
                        fprintf(stderr, "signal_handling_func did not find a handler for %d\n", sig);
                    }
                }
            } else {
                fprintf(stderr, "signal_handling_func did not find a sigaction for %d\n", sig);
            }
        }
    }

    signal_thread_started = 0;
    return NULL;
}

static void graalos_update_signal_handler_thread(int sig, int installed) {
    if (installed) {
        sigaddset(&signal_thread_waitset, sig);

        int started = signal_thread_started;
        if (!started && !a_cas(&signal_thread_started, started, 1)) {
            // ensure that signal handling thread wakes on SIGSYS
            sigaddset(&signal_thread_waitset, SIGSYS);

            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

            pthread_t thread;
            pthread_create(&thread, &attr, signal_handling_func, &signal_thread_waitset);
        } else {
            // either signal handling thread was already running or another thread beat us to
            // starting it... just wake it up with SIGSYS so it re-issues its signal wait
            raise(SIGSYS);
        }
    } else {
        sigdelset(&signal_thread_waitset, sig);
        raise(SIGSYS); // wake signal handling thread to re-issue its signal wait
    }
}

int sigaction(int sig, const struct sigaction* restrict sa, struct sigaction* restrict old) {
    // call core MUSL implementation
    int result = __sigaction(sig, sa, old);

    if (sa && !result) {
        graalos_update_signal_handler_thread(sig, (uintptr_t)sa->sa_handler > 1UL);
    }

    return result;
}
