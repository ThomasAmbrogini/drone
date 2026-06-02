#include "coco/os/sleep.hh"

#include "stm32f4xx.h"

namespace os {

void sleep_busy_wait_us(int duration_us) {
    int duration_ticks {static_cast<int>(static_cast<uint64_t>(duration_us) * SystemCoreClock / 1000000)};
    int elapsed_time_ticks {};
    uint32_t load_value {SysTick->LOAD};

    uint32_t time_start {SysTick->VAL};
    while (elapsed_time_ticks < duration_ticks) {
        uint32_t time_now {SysTick->VAL};

        if (time_now < time_start) {
            elapsed_time_ticks += time_start - time_now;
        } else {
            elapsed_time_ticks += load_value - time_now + time_start + 1;
        }

        time_start = time_now;
    }
}

void sleep_busy_wait_ms(int duration_ms) {
    sleep_busy_wait_us(duration_ms * 1000);
}

} /* namespace os */

