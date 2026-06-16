#pragma once

namespace coco {

inline void assert(bool expression) {
#ifdef DEBUG
    if (!expression) {
        //TODO: It would be kinda interesting to print something.
        __builtin_trap();
    }
#endif
}

} /* namespace coco */

