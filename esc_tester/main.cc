#include "coco/os/platform.hh"
#include "coco/os/sleep.hh"

#include "servo_control.hh"

int main() {
    os::platform_init();
    os::sleep_busy_wait_ms(1000);

    return 5;
}

