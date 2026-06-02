#include "coco/os/platform.hh"

#include "stm32f4xx.h"

namespace os {
    void platform_init() {
          SysTick->LOAD  = 0x00FFFFFF;
          SysTick->VAL   = 0UL;
          SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                           SysTick_CTRL_ENABLE_Msk;

    }
} /* namespace os */

