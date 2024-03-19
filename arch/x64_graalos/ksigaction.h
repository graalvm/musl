#include <features.h>

#include "include/graalos/musl_sigaction.h"

hidden void __restore_rt();
#define __restore __restore_rt
