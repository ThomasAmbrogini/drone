#include "coco/os/sleep.hh"

#include <unistd.h>

namespace os {

void sleep_busy_wait_us(int duration_us) {
    usleep(static_cast<useconds_t>(duration_us));
}

void sleep_busy_wait_ms(int duration_ms) {
    sleep_busy_wait_us(duration_ms * 1000);
}

} /* namespace os */


