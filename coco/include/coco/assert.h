#pragma once

#ifdef DEBUG
#define COCO_ASSERT(expression) \
    do {                        \
        if (!(expression)) {    \
            __builtin_trap();   \
        }                       \
    } while (0)
#else
#define COCO_ASSERT(expression) ((void)0)
#endif

