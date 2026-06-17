#include "coco/os/platform.hh"
#include "coco/os/sleep.hh"
#include "coco/hal/pwm.hh"

#include "servo_control.hh"

int main() {
    os::platform_init();
    os::sleep_busy_wait_ms(1000);

    auto PWMInstance {coco::pwm_retrieve_instance(coco::pwm_instance::_1)};
    coco::pwm_init(PWMInstance);
    coco::pwm_enable(PWMInstance);

    return 5;
}

